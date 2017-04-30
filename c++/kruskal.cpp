#include "kruskal.h"







kruskal::kruskal(Graph<Node *> & graph, Node * start, Node * end){
    Vector <Vector<Node *> clusters;
    PriorityQueue<Edge *> edges;

    clusters.add(graph.node);

    for(Edge * edge : graph) {
        edges.enqueue()
    }

    while(clusterSets.size() >= 2){
        Edge * currentEdge = clusters.dequeue();
        if(!clusters[currentEdge->start, currentEdge->finish]){

        }

     }

    for()

    for(Node * node : graph.clusters){
        Node * currentCluster = node;
        clusters.add(currentCluster);

    }
    bool inSameCluster(Set<Set <Node *> > clusterSets) {
        for(Set<Node*> cluster : clusterSets){
            if(cluster.contains(start)){
               cluster.contains(end);
               return true;
            }
            return false;
        }
    }
}
