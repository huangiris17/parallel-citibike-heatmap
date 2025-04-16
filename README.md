# NYC Citi Bike Heatmap

This project processes and visualizes NYC Citi Bike trip data. It includes data preprocessing with Python, data aggregation using C++ with OpenMP, and final visualization as an interactive heatmap.

## Project Structure

- `scripts/preprocess.py`: Preprocesses raw Citi Bike data
- `cpp/aggregation.cpp`: Aggregates trip data in parallel using OpenMP
- `scripts/visualize.py`: Generates a heatmap from aggregated data
- `nyc_citibike_heatmap.html`: Output HTML file for visualization

## Requirements

- Python 3
- C++ compiler with OpenMP support (e.g., `g++-14`)
- macOS (for `open` command, or modify accordingly for other OS)

## Setup

Create a Python virtual environment and install dependencies:

```bash
make setup
```

## Preprocess Data

Run the Python script to preprocess raw data:

```bash
make preprocess
```

## Build Aggregation Binary

Compile the C++ aggregation program:

```bash
make build
```

## Run Aggregation

Execute the compiled C++ program:

```bash
make run
```

## Visualize Heatmap

Generate the heatmap using the processed data:

```bash
make visualize
```

## Open Heatmap

Render the heatmap in your default browser (macOS):

```bash
make open
```