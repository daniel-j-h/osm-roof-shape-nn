#include <osmium/io/reader.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/visitor.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>

#include <boost/geometry.hpp>

#include <cstring>
#include <cstdlib>

#include <ios>
#include <locale>
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>


// No need for associative container or binary search; class is index [0,n-1]
// http://wiki.openstreetmap.org/wiki/Simple_3D_buildings#Roof
// http://taginfo.openstreetmap.org/keys/roof%3Ashape#values
static const std::vector<std::string> roofClasses{
    {"flat"},
    {"skillion"},
    {"gabled"},
    {"half-hipped"},
    {"hipped"},
    {"pyramidal"},
    {"gambrel"},
    {"mansard"},
    {"dome"},
    {"onion"},
    {"round"},
    {"saltbox"},
};


// Detects roofs and extracts (lat,lon,class) centroid tuples, piping them into the stream
struct BuildingHandler final : osmium::handler::Handler {
  explicit BuildingHandler(std::ostream& out_) : out{out_} {}

  // Handle ways
  void way(const osmium::Way& way) {
    const auto* roof = way.get_value_by_key("roof:shape");
    if (not roof)
      return;

    const auto it = std::find(std::begin(roofClasses), std::end(roofClasses), roof);
    if (it == std::end(roofClasses))
      return;

    const auto roofClass = std::distance(std::begin(roofClasses), it);

    // Report simple centroid of building way nodes
    using Point = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;
    using Line = boost::geometry::model::linestring<Point>;

    Line line;
    for (const auto& node : way.nodes())
      boost::geometry::append(line, Point{node.location().lon(), node.location().lat()});

    Point center;
    boost::geometry::centroid(line, center);

    const auto longitude = boost::geometry::get<0>(center);
    const auto latitude = boost::geometry::get<1>(center);

    out << std::fixed << latitude << ',' << longitude << ',' << roofClass << '\n';
  }

  // Handle areas
  void area(const osmium::Area& /*area*/) {
    // TODO; ways should give us most of the roof shapes
  }

  std::ostream& out;
};


int main(int argc, char** argv) try {
  std::locale::global(std::locale(""));
  std::ios_base::sync_with_stdio(false);

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " in.osm.pbf" << std::endl;
    return EXIT_FAILURE;
  }

  // Buildings can be mapped based on areas or ways
  osmium::io::Reader extractReader(argv[1]);

  // Locations need to be cached otherwise we get empty refs
  using PositiveIndex = osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
  using LocationHandler = osmium::handler::NodeLocationsForWays<PositiveIndex>;

  PositiveIndex positiveIndex;
  LocationHandler locationHandler{positiveIndex};

  // Extracts buildings
  BuildingHandler buildingHandler{std::cout};

  // Run pipeline
  osmium::apply(extractReader, locationHandler, buildingHandler);

} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << std::endl;
  return EXIT_FAILURE;
}
