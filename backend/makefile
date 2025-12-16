build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp buildings.cpp node.cpp nodes.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp busstop.h busstop.cpp busstops.h busstops.cpp -Wno-unused-variable -Wno-unused-function -lcurl

build-offline:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall -DOFFLINE main.cpp building.cpp buildings.cpp node.cpp nodes.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl

build-online-save:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall -DSAVE_ONLINE_RESPONSES main.cpp building.cpp buildings.cpp node.cpp nodes.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp buildings.cpp node.cpp nodes.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp busstop.h busstop.cpp busstops.h busstops.cpp -Wno-unused-variable -Wno-unused-function -lcurl
	valgrind --tool=memcheck --leak-check=full ./a.out

build-server:
	rm -f ./server
	g++ -std=c++17 -g -Wall -pthread server.cpp api_helpers.cpp building.cpp buildings.cpp node.cpp nodes.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp busstop.cpp busstops.cpp -Wno-unused-variable -Wno-unused-function -lcurl -o server

run-server:
	./server

clean:
	rm -f ./a.out ./server

commit:
	git add .
	git commit -m "committed changes"