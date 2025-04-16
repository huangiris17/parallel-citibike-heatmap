# Makefile to run data preprocessing and build

# Set up virtual python env
setup:
	python3 -m venv .venv && \
	source .venv/bin/activate && \
	pip install -r requirements.txt

# Run Python preprocessing script
preprocess:
	source .venv/bin/activate && python3 scripts/preprocess.py

# Build C++ code (delegate to cpp/Makefile)
build:
	g++-14 -std=c++17 -fopenmp cpp/aggregation.cpp -o aggregation

# Run the C++ aggregation code
run:
	./aggregation

# Visualize the data in a heatmap
visualize:
	source .venv/bin/activate && python3 scripts/visualize.py

# Render the heatmap html
open:
	open nyc_citibike_heatmap.html

# Run all commands together
all: setup preprocess build run visualize open