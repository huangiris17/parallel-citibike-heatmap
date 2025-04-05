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

run:
	./aggregation

# Clean C++ build artifacts
clean:
	$(MAKE) -C cpp clean

# Full pipeline: preprocess -> build -> run
# run: preprocess build
# 	./cpp/bin/bike_analysis
