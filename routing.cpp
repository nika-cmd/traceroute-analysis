/* CPSC441 - Assignment3
 * 
 * Author: Thessalonika Magadia
 * Submitted: 19 Nov 2021
 * 
 * References:
 *  [1] J.F. Kurose and K.W. Ross. Chapter 5 Network Layer: Control Plane [PowerPoint slides]
 *      https://d2l.ucalgary.ca/d2l/le/content/401094/viewContent/4802386/View (10 Nov 2021)
 *  [2] List of international airports in Canada [Wikipedia]
 *      https://en.wikipedia.org/wiki/List_of_international_airports_in_Canada (18 Nov 2021)
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#define INFINITY 1000000;

using namespace std;

struct Link
{
    string src;
    string dst;
    int time;
};

struct Table
{
    string node; // aka destination
    vector<Link> links; // links for that path
    int time_to_node = INFINITY; // minimum time to current node
};

// find index of table with this nod  (first occurence)
int find_index (const vector<Table> &s, string n) {
    if (s.empty()) {
        return -1;
    }
    for (int i = 0; i < s.size(); i++) {
        if (s.at(i).node.compare(n) == 0) {
            return i;
        }
    }
    return -1;
}

// checks if vecotr of Other_Node already contains node (string)
bool table_contains_node (const vector<Table> &s, string airport_code) 
{
    for (int i = 0; i < s.size(); i++) {

        if (airport_code.compare(s.at(i).node) == 0) 
            return true; // contains this node

    }
    return false; // does not contain node
}

// cehcks if vector<string> contains the string a
bool list_contains_node (const vector<string> &s, string a) 
{
    for (int i = 0; i < s.size(); i++) {

        // if s contains string a
        if (s.at(i).compare(a) == 0) {
            return true;
        }

    }
    return false;
}

// converts airport code to corresponding city [2]
string airport_to_city (string code) {

    if (code.compare("YEG") == 0) 
        return "Edmonton";
    else if (code.compare("YQR") == 0)
        return "Regina";
    else if (code.compare("YXE") == 0)
        return "Saskatoon";
    else if (code.compare("YVR") == 0)
        return "Vancouver";
    else if (code.compare("YWG") == 0)
        return "Winnipeg";
    else if (code.compare("YUL") == 0)
        return "Montreal";
    else if (code.compare("YYZ") == 0)
        return "Toronto  ";
    else if (code.compare("YYJ") == 0)
        return "Victoria";
    else if (code.compare("YOW") == 0) 
        return "Ottawa";
    else if (code.compare("YQB") == 0)
        return "Quebec City";
    else if (code.compare("YHZ") == 0)
        return "Halifax";
    else if (code.compare("YQM") == 0)
        return "Moncton";
    else if (code.compare("YFC") == 0)
        return "Fredericton";
    else if (code.compare("YQX") == 0)
        return "Gander";
    else if (code.compare("YYT") == 0)
        return "St. John's";
    else if (code.compare("YXY") == 0)
        return "Whitehorse";
    else if (code.compare("YZF") == 0)
        return "Yellowknife";
    else if (code.compare("YFB") == 0)
        return "Iqaluit";
    else if (code.compare("YXX") == 0)
        return "Abbotsford";
    else if (code.compare("YXS") == 0)
        return "Prince George";
    else if (code.compare("YYC") == 0)
        return "Calgary";
    
    return string();
    
}

// print table
void print_table (const vector<Table> &s) {
    cout << endl << "Destination\t" << "Cost\t" << "Hops\t" << "Shortest Path";
    for (int i = 0; i < s.size(); i++) {
        cout << endl << s.at(i).node << "\t\t" << s.at(i).time_to_node 
            << "\t" << s.at(i).links.size() << "\t" ;
        if (s.at(i).links.empty()) {
            cout << "<empty>";
        }
        else {
            cout << s.at(i).links.at(0).src; // initial source
            for (int j = 0; j < s.at(i).links.size(); j++) {
                cout << " -> " << s.at(i).links.at(j).dst;
            }    
        }
        
    }
}

// prints results
void print_result (const Table &s) {
    cout << endl << airport_to_city(s.node) << "\t" << s.time_to_node << "\t"
        << s.links.size() << "\t";
    cout << s.links.at(0).src;
    for (int i = 0; i < s.links.size(); i++) {
        cout << " -> " << s.links.at(i).dst;
    }
}


// Dijkstra's link-state routing algorithm [1]
// PROMISES: to find the least cost path between source and destination
//  then returns vector<Table> of least-cost-path to all other possible nodes
vector<Table> routing_algorithm (string initial_src, const vector<Link> &map) 
{
    vector<Table> best_route; // stores the current best route from initial source node to other node
    vector<string> visited_nodes;

    // initialize table of best route with other nodes (excludes initial source node)
    for (int i = 0; i < map.size(); i++) {

        // if best_route does not contain this node (dst)
        if ( !table_contains_node(best_route, map.at(i).dst) && initial_src.compare(map.at(i).dst) != 0) {
            struct Table n;
            n.node = map.at(i).dst;
            best_route.push_back (n);
        } 
       
       // if best_route does not contain this node (src)
        if ( !table_contains_node(best_route, map.at(i).src) && initial_src.compare(map.at(i).src) != 0) {
            struct Table n;
            n.node = map.at(i).src;
            best_route.push_back (n);
        } 
        

    }

    // initialization of least-cost-path of neighbor nodes
    for (int i = 0; i < map.size(); i++){

        if (initial_src.compare(map.at(i).src) ==  0) {
            
            for (int j = 0; j < best_route.size(); j++) {

                if (map.at(i).dst.compare(best_route.at(j).node) == 0) {
                    best_route.at(j).time_to_node = map.at(i).time;
                    best_route.at(j).links.push_back (map.at(i));
                }

            }
            
        }
        
    }
  
    visited_nodes.push_back(initial_src); // add initial node to visited nodes
    
    
    // finding the least-cost-path from source node to destination node
    while (visited_nodes.size() < best_route.size()) {
        
        /*cout << "\n>>> visited nodes: ";
        for (int i = 0; i < visited_nodes.size(); i++) {
            cout << visited_nodes.at(i) << " ";
        }*/ 
        
        struct Table current_node; // node that is next to be visited (current minimum cost)

        // find node NOT in visted_nodes such that trip time is a minimum
        for (int i = 0; i < best_route.size(); i++) {
            
            // check if node has not been visited, and if node is empty it will set the node first
            if ( (!list_contains_node(visited_nodes, best_route.at(i).node)) && current_node.node.empty()) {
                current_node = best_route.at(i);
            }
            else if ( (!list_contains_node(visited_nodes, best_route.at(i).node)) && (!current_node.node.empty()) && (best_route.at(i).time_to_node < current_node.time_to_node)) {
                current_node = best_route.at(i);
            }
        }

        // add this node to visited nodes
        visited_nodes.push_back(current_node.node);

        // update least-cost-path to all nodes adjacent to min node that have not been visited
        for (int i = 0; i < map.size(); i++) {

            // node src next node and
            if (current_node.node.compare(map.at(i).src) == 0 && !list_contains_node(visited_nodes, map.at(i).dst)) {
                
                int current_node_index = find_index (best_route, current_node.node);
                int adj_node_index = find_index (best_route, map.at(i).dst);

                // if new route is less than currenlty known least-cost-path, update
                if (best_route.at(current_node_index).time_to_node + map.at(i).time < best_route.at(adj_node_index).time_to_node) {
                    //cout << "\n current time: " << best_route.at(adj_node_index).time_to_node;
                    //cout << "\n new time: " << best_route.at(current_node_index).time_to_node + map.at(i).time;
                    best_route.at(adj_node_index).time_to_node = best_route.at(current_node_index).time_to_node + map.at(i).time;
                    best_route.at(adj_node_index).links.clear(); // clears current link path
                    best_route.at(adj_node_index).links.assign(best_route.at(current_node_index).links.begin(), 
                        best_route.at(current_node_index).links.end()); // copy in path to current node
                    best_route.at(adj_node_index).links.push_back(map.at(i)); // add path from current node to adjacent
                }
            }

        }  
        
    } 

    //cout << "\n<< TABLE OF LEAST-COST-PATH TO ALL NODES >>";
    //print_table(best_route); // for debugging

    
    return best_route;
}

int main (void) 
{

    // list of nodes
    vector<Link> map_of_nodes;

    // reading the file with all the nodes
    ifstream myFile;
    myFile.open ("map_of_canada.txt");

    if (!myFile.is_open()) {
        cout << "File could not be found!" << endl;
    }

    string buffer;
    while (getline(myFile, buffer)) {

        // saving nodes from file into node struct
        struct Link n;
        n.src = buffer.substr(0,3);
        n.dst = buffer.substr(4,3);
        n.time = stoi(buffer.substr(8)); 

        // add Node at end
        map_of_nodes.push_back(n); 

    }

    // print out map_of_nodes for debugging
    /* for (int i = 0; i < map_of_nodes.size(); i++) {
        cout << i+1 << " " << map_of_nodes.at(i).src << " " << map_of_nodes.at(i).dst
            << " " << map_of_nodes.at(i).time << endl;

    } */


    // Dijkstra's link-state routing algorithm

    // prints least-cost-path from YYC to all other nodes
    cout << "\n<< TABLE OF LEAST-COST-PATH TO ALL OTHER NODES >>";
    vector <Table> table = routing_algorithm("YYC", map_of_nodes);
    print_table(table); // prints lesat-cost-path from YYC to all other nodes
    
    // print tabular summary
    cout << "\n\n<< RESULTS SUMMARY >>";
    cout << endl << "Destination\t" << "Cost\t" << "Hops\t" << "Shortest Path";
    int result_index = find_index(table, "YVR");
    print_result (table.at(result_index)); // Vancouver
    result_index = find_index(table, "YYZ");
    print_result (table.at(result_index)); // Toronto
    result_index = find_index(table, "YWG");
    print_result (table.at(result_index)); // Winnipeg
    cout << endl;
    
    return 0;
    
}
