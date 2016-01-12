### osm-roof-shape-nn

Learning and Predicting Roof Shapes from OpenStreetMap data.

> "probably the most badass use of the static maps python sdk to date!"
-- Matthew Perry

## Getting Started

In the root directory, create and acivate a venv:

    virtualenv --system-site-packages .env
    . .env/bin/activate.fish

In subproject directories:

    pip install -r Requirements.txt

## Usage

Extract locations which have tagged roof shapes: save (latitude, longitude, shape) tuple:

    ./ExtractBuildings OpenStreetMap/niederbayern-latest.osm.pbf > /tmp/RoofLocations.csv

Download satellite images from the static map Mapbox service based on those locations:

    export MAPBOX_ACCESS_TOKEN='YOUR-MAPBOX-ACCESS-TOKEN' 
    mkdir /tmp/RoofImages
    ./GenerateImages /tmp/RoofLocations.csv /tmp/RoofImages/

Run the artificial neural net on the already classified images:

On the CPU:

    export THEANO_FLAGS=device=cpu,openmp=True,floatX=float32

On GPUS:

    export THEANO_FLAGS=device=gpu,floatX=float32

Train it:

    ./Neuralnet /tmp/RoofImages/


## License

Copyright © 2015 Daniel J. Hofmann

Distributed under the MIT License (MIT).
