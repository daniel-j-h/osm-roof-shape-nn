#!/usr/bin/env bash

set -e

OSMURLS=(
  "http://download.geofabrik.de/europe/germany/bayern/niederbayern-latest.osm.pbf"
)

OSMDIR="OpenStreetMap"

mkdir -p ${OSMDIR}
pushd ${OSMDIR}
for URL in ${OSMURLS[@]}; do wget --quiet ${URL}; done
popd
