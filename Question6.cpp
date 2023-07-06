#include <bits/stdc++.h>
#include <chrono>
using namespace std;
// DSU data structure
// path compression + rank by union
class DSU {
    int* parent;
    int* rank;
 
public:
    DSU(int n)
    {
        parent = new int[n];
        rank = new int[n];
 
        for (int i = 0; i < n; i++) {
            parent[i] = -1;
            rank[i] = 1;
        }
    }
 
    // Find function
    int find(int i)
    {
        if (parent[i] == -1)
            return i;
 
        return parent[i] = find(parent[i]);
    }
 
    // Union function
    void unite(int x, int y)
    {
        int s1 = find(x);
        int s2 = find(y);
 
        if (s1 != s2) {
            if (rank[s1] < rank[s2]) {
                parent[s1] = s2;
            }
            else if (rank[s1] > rank[s2]) {
                parent[s2] = s1;
            }
            else {
                parent[s2] = s1;
                rank[s1] += 1;
            }
        }
    }
};

//graphs are objects of a class
class graph{
    int size;
    vector<vector<pair<int,float>>> adj;
    vector<pair<float,pair<int,int>>> edges;
    vector<vector<float>> dist;
    vector<vector<int>> par;
    vector<float> excentricity;
public:
    graph(int n):size(n),adj(size),dist(size),par(size),excentricity(size){}
    void addEdge(int a, int b, float c){
        adj[a-1].push_back(make_pair(b-1,c));
        adj[b-1].push_back(make_pair(a-1,c));
        edges.push_back({c,{a-1,b-1}});
        return;
    }
    void dijkstra(int root){
        if(dist[root-1].size()>0)return;//if dijkstra was already calculated with this root, return
        float exc = 0; //tweak on the dijkstras algorithm to calculate the excentricity during it.
                       //we just need to keep track of the greatest distance so far
        dist[root-1].assign(size,numeric_limits<float>::max());//set distances to infinity
        par[root-1].assign(size,0);
        //distance from itself is 0, and has no parent
        dist[root-1][root-1] = 0;
        par[root-1][root-1] = -1;
        //dijkstras algorithm uses a priority queue to traverse the closest nodes first
        priority_queue<pair<float,int>> pq;
        pq.push({0,root-1});
        //for each node on the priority queue, we visit all of his adjacencies
        //changing the distance and parent if the adjacencies are closer to the root by passing through
        //the node than it was before, then push them to the pq
        while(!pq.empty()){
            float adist = -pq.top().first;
            int a = pq.top().second;
            pq.pop();
            if(dist[root-1][a]<adist)continue;
            for(int x = 0; x<adj[a].size();x++){
                int v = adj[a][x].first;
                float w = adj[a][x].second;
                if(dist[root-1][v]>dist[root-1][a]+w){
                    par[root-1][v] = a;
                    dist[root-1][v] = dist[root-1][a] + w;
                    pq.push({-dist[root-1][v],v});
                    if(dist[root-1][v]>exc){exc = dist[root-1][v];}//keeping track of the greatest distance
                }
            }
        }
        excentricity[root-1] = exc;
        return;
    }
    float getDist(int start, int end){
        if(dist[start-1].size()==0){dijkstra(start);}
        return dist[start-1][end-1];
    }
    float getExcentricity(int node){
        if(excentricity[node-1] == 0){dijkstra(node);}
        return excentricity[node-1];
    }
    void findPath(int start, int end){
        if(par[start-1].size() == 0){dijkstra(start);}
        //we just go through the parents vector, 
        //jumping from the node to its parent recursively until it reaches the root
        int i = end - 1;
        vector<int> path;
        std::cout<<"Path from node " << start << " to node " << end <<endl;
        while(par[start-1][i]!=-1){
            path.push_back(i+1);
            i = par[start-1][i];
        }
        path.push_back(start);
        for(int j  = 1; j <= path.size(); j++){
            std::cout << path[path.size()-j] << " ";
        }
        std::cout << endl;
        return;
    }
    void kruskal(){
        if(edges.size()<size-1){std::cout << "Disconnected Graph\n"; return;}
        else if(edges.size() > size-1){
            sort(edges.begin(),edges.end());
            DSU kruskals(size);
            float minCost = 0.0;
            for(int i = 0; i < edges.size(); i++){
                float w = edges[i].first;
                int a = edges[i].second.first;
                int b = edges[i].second.second;
                if(kruskals.find(a) != kruskals.find(b)){
                    kruskals.unite(a,b);
                    minCost += w;
                    std::cout << "Edge " << a << " - " << b << " Cost " << w << endl;
                }
            }
            std::cout << "\nTotal cost of MST is " << minCost << "\n" << endl;
            return;
        }
        else{
            std::cout << "The graph is already a minimum spanning tree\n" << endl;
            float minCost = 0.0;
            for(int i = 0; i < edges.size(); i++){
                float w = edges[i].first;
                int a = edges[i].second.first;
                int b = edges[i].second.second;
                std::cout << "Edge " << a << " - " << b << " Cost " << w << endl;
                minCost += w;
            }
            std::cout << "\nTotal cost of MST is " << minCost << "\n" << endl;
            return;
        }
    }
};


int main() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    ifstream input;
    input.exceptions(ifstream::badbit);
    try{
        input.open("grafo_W_1.txt");
        int n;
        input >> n;
        graph W1(n);
        while(!input.eof()){
            int a = 0, b = 0; 
            float c = 0.0;
            input >> a >> b >> c;
            if(a<=0 or a>=n or b<=0 or b>=n)continue;
            W1.addEdge(a,b,c);
        }
        std::cout << "Graph W1: \n\n";
        std::cout << "A) Distance and path \n";
        auto sa = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W1.getDist(1,i) << endl;
            W1.findPath(1,i);
            std::cout << endl;
        }
        auto ea = high_resolution_clock::now();
        duration<double, std::milli> ta = ea - sa;
        std::cout << "Elapsed time: " << ta.count() << " ms\n" << endl;
        auto sb = high_resolution_clock::now();
        std::cout << "B) Minimum spanning tree \n";
        W1.kruskal();
        auto eb = high_resolution_clock::now();
        duration<double, std::milli> tb = eb - sb;
        std::cout << "Elapsed time: " << tb.count() << " ms\n" << endl;
        std::cout << "C) Excentricity \n"; 
        auto sc = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W1.getExcentricity(i) << "\n" << endl;
        }
        auto ec = high_resolution_clock::now();
        duration<double, std::milli> tc = ec - sc;
        std::cout << "Elapsed time: " << tc.count() << " ms\n" << endl;
        input.close();
    }
    catch(const ifstream::failure& e){
        std::cout << "Exception in file Grafos_W_1.txt";
    }

    try{
        input.open("grafo_W_2.txt");
        int n;
        input >> n;
        graph W2(n);
        while(!input.eof()){
            int a = 0, b = 0; 
            float c = 0.0;
            input >> a >> b >> c;
            if(a<=0 or a>=n or b<=0 or b>=n)continue;
            W2.addEdge(a,b,c);
        }
        std::cout << "Graph W2: \n\n";
        std::cout << "A) Distance and path \n";
        auto sa = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W2.getDist(1,i) << endl;
            W2.findPath(1,i);
            std::cout << endl;
        }
        auto ea = high_resolution_clock::now();
        duration<double, std::milli> ta = ea - sa;
        std::cout << "Elapsed time: " << ta.count() << " ms\n" << endl;
        auto sb = high_resolution_clock::now();
        std::cout << "B) Minimum spanning tree \n";
        W2.kruskal();
        auto eb = high_resolution_clock::now();
        duration<double, std::milli> tb = eb - sb;
        std::cout << "Elapsed time: " << tb.count() << " ms\n" << endl;
        std::cout << "C) Excentricity \n"; 
        auto sc = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W2.getExcentricity(i) << "\n" << endl;
        }
        auto ec = high_resolution_clock::now();
        duration<double, std::milli> tc = ec - sc;
        std::cout << "Elapsed time: " << tc.count() << " ms\n" << endl;
        input.close();
    }
    catch(const ifstream::failure& e){
        std::cout << "Exception in file Grafos_W_2.txt";
    }
    try{
        input.open("grafo_W_3.txt");
        int n;
        input >> n;
        graph W3(n);
        while(!input.eof()){
            int a = 0, b = 0; 
            float c = 0.0;
            input >> a >> b >> c;
            if(a<=0 or a>=n or b<=0 or b>=n)continue;
            W3.addEdge(a,b,c);
        }
        std::cout << "Graph W3: \n\n";
        std::cout << "A) Distance and path \n";
        auto sa = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W3.getDist(1,i) << endl;
            W3.findPath(1,i);
            std::cout << endl;
        }
        auto ea = high_resolution_clock::now();
        duration<double, std::milli> ta = ea - sa;
        std::cout << "Elapsed time: " << ta.count() << " ms\n" << endl;
        auto sb = high_resolution_clock::now();
        std::cout << "B) Minimum spanning tree \n";
        W3.kruskal();
        auto eb = high_resolution_clock::now();
        duration<double, std::milli> tb = eb - sb;
        std::cout << "Elapsed time: " << tb.count() << " ms\n" << endl;
        std::cout << "C) Excentricity \n"; 
        auto sc = high_resolution_clock::now();
        for(int i = 10; i < 50; i+=10){
            std::cout << i << ": " << W3.getExcentricity(i) << "\n" << endl;
        }
        auto ec = high_resolution_clock::now();
        duration<double, std::milli> tc = ec - sc;
        std::cout << "Elapsed time: " << tc.count() << " ms\n" << endl;
        input.close();
    }
    catch(const ifstream::failure& e){
        std::cout << "Exception in file Grafos_W_3.txt";
    }
    return 0;
}
