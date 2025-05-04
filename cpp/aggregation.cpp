#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tbb/concurrent_unordered_map.h>
#include <chrono>
#include <omp.h>
#include <utility>

struct Trip {
    std::string hour;
    std::string station_id;
};

std::vector<Trip> loadTripCSV(const std::string& path) {
    std::vector<Trip> tripVec;
    std::ifstream file;
    std::string line;

    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return tripVec;
    }

    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream strStream(line);
        std::string hourStr, stationId;

        std::getline(strStream, hourStr, ',');
        std::getline(strStream, stationId, ',');

        if (!hourStr.empty() && !stationId.empty()) {
            tripVec.push_back({hourStr, stationId});
        }
    }

    return tripVec;
}

std::unordered_map<std::string, int> countTripSequential(std::vector<Trip> tripVec) {
    std::unordered_map<std::string, int> countMap;

    for (const auto& trip: tripVec) {
        std::string key = trip.hour + "-" + trip.station_id;
        countMap[key]++;
    }

    return countMap;
}

namespace std {
    template <>
    struct hash<std::pair<std::string, std::string>> {
        size_t operator()(const std::pair<std::string, std::string>& k) const {
            return hash<std::string>()(k.first) ^ (hash<std::string>()(k.second) << 1);
        }
    };
}
tbb::concurrent_unordered_map<std::pair<std::string, std::string>, int> countTripParallel(std::vector<Trip> tripVec) {
    int nThreads = omp_get_max_threads();
    std::cout << "Number of threads used: " << nThreads << std::endl;
	tbb::concurrent_unordered_map<std::pair<std::string, std::string>, int> countMap;

        #pragma omp for
        for (int i = 0; i < tripVec.size(); i++) {
			const Trip& trip = tripVec[i];   
			auto key = std::make_pair(trip.hour, trip.station_id);            
			localMap[key]++;
        }
	
	/*	
    #pragma omp parallel
    {
        std::unordered_map<std::string, int> localMerge;

        #pragma omp for nowait
        for (int i = 0; i < countMapLocal.size(); ++i) {
            for (const auto& [key, count] : countMapLocal[i]) {
                localMerge[key] += count;
            }
        }

        #pragma omp critical
        {
            for (const auto& [key, count] : localMerge) {
                countMap[key] += count;
            }
        }
    }
	**/

	/*
	#pragma omp parallel for
	for (int i = 0; i < countMapLocal.size(); ++i) {
    	for (const auto& [key, count] : countMapLocal[i]) {
        	#pragma omp critical
        	countMap[key] += count;
    	}
	}
	**/
	
	/*	
	for (const auto& localMap : countMapLocal) {
 	   for (const auto& [key, count] : localMap) {
    	    countMap[key] += count;
    	}
	}
	**/
	

    return countMap;
}

void writeToCSV(const std::unordered_map<std::string, int>& countMap, const std::string& path) {
    std::ofstream file;
    file.open(path);

    file << "hour,start_station_id,ride_count\n";

    for (const auto& [key, count]: countMap) {
        auto delimIdx = key.find('-');
        std::string hour = key.substr(0, delimIdx);
        std::string station_id = key.substr(delimIdx + 1);
        file << hour << "," << station_id << "," << count << "\n";
    }
}

int main() {
    std::string inputFile = "data/processed/hourly_usage.csv";
    std::string outputFile_sequantial = "data/aggregated/hourly_usage_seq.csv";
    std::string outputFile_parallel = "data/aggregated/hourly_usage_par.csv";

    auto trips = loadTripCSV(inputFile);
    std::cout << "Loaded " << trips.size() << " trip records.\n";

    auto start_sequential = std::chrono::high_resolution_clock::now();
    auto result_sequetial = countTripSequential(trips);
    auto end_sequential = std::chrono::high_resolution_clock::now();

    auto duration_sequential = std::chrono::duration_cast<std::chrono::milliseconds>(end_sequential - start_sequential);
    std::cout << "Sequential Time: " << duration_sequential.count() << " ms\n";

    auto start_parallel = std::chrono::high_resolution_clock::now();
    auto result_parallel = countTripParallel(trips);
    auto end_parallel = std::chrono::high_resolution_clock::now();

    auto duration_parallel = std::chrono::duration_cast<std::chrono::milliseconds>(end_parallel - start_parallel);
    std::cout << "Parallel Time: " << duration_parallel.count() << " ms\n";

    double speedup = static_cast<double>(duration_sequential.count()) / duration_parallel.count();
    std::cout << "Speedup: " << speedup << std::endl;

    writeToCSV(result_sequetial, outputFile_sequantial);
    std::cout << "Sequential aggregation results saved" << std::endl;
    writeToCSV(result_parallel, outputFile_parallel);
    std::cout << "Parallel aggregation results saved" << std::endl;

    return 0;
}
