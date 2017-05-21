// Ah... The joys graph data structures :)
// A hole into which many a good computer scientist has fallen, never to be heard from again.
//					- Andrew Sutton

/*
	Basic idea: provide concepts that define the interface(s) to different kinds of graphs so that you can do
	basic graph operations and algoriths without knowing exactly which kind of graph it is and keep ignorant
	about implementation details.
    
    Basic design idea: do like the STL
*/

/*
// Graph concepts (like STL containers):
// Do we need them (STL doesn't make containers explicit)
template<class G> concept bool Graph = false; // general graph operations
template<class G> concept bool DAG = false;	 // operations simplified for DAGs (any extra operations?)
template<class G> concept bool Tree = false;	 // operations simplified for trees (any extra operations?)

// accessor concepts (like STL Iterators):
template<class G> concept bool Edge_ref = false;	     // most general and slowest
template<class G> concept bool DAG_edge_ref = false;	 // operations simplified for DAGs (any extra operations?)
template<class G> concept bool Tree_edge_ref = false; // operations simplified for trees (any extra operations?)

template<class G> concept bool Vertex_ref = false;      // most general and slowest
template<class G> concept bool DAG_vertex_ref = false;  // operations simplified for DAGs (any extra operations?)
template<class G> concept bool Tree_vertex_ref = false; // operations simplified for DAGs (any extra operations?)

// the value type (in a more general design, this would be a template parmeter):
struct Val {};

// specific directed graphs:
struct Tree {};
struct Dag { };
struct Dgraph {};

struct Node_ref {};
struct Edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

struct DAG_vertex_ref {};
struct DAG_edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

struct Gnode_ref {};
struct Gedge_ref {};	// Is an Edge an object? (if not, refer to parent node)

// another Graph representation:
struct DGN_ref {};
struct DGE_ref {};	// Is an Edge an object? (if not, refer to parent node)

// use:
template<Graph G>
void traverse(G& g)
{
	vector<???> found;	// there is a way (look up traits), lets try g::value_type
}

*/

/*
	Basic idea: provide concepts that define the interface(s) to different kinds of graphs so that you can do
	basic graph operations and algoriths without knowing exactly which kind of graph it is and keep ignorant
	about implementation details.
    
    Basic design idea: do like the STL
*/

/*
// Graph concepts (like STL containers):
// Do we need them (STL doesn't make containers explicit)
template<class G> concept bool Graph =  // general graph operations
	requires { typename G::value_type; };
template<class G> concept bool DAG = Graph<G> && requires(G g) { tops(g); };	 // operations simplified for DAGs 
template<class G> concept bool Tree = DAG<G> && requires(G g) { top(g); };	 // operations simplified for trees

// accessor concepts (like STL Iterators):
template<class E> concept bool Edge_ref =      // most general and slowest
	requires { typename E::value_type; };
template<class E> concept bool DAG_edge_ref = // operations simplified for DAGs (any extra operations?)
	Edge_ref<E> && false;	 
template<class E> concept bool Tree_edge_ref = // operations simplified for trees (any extra operations?)
	DAG_edge_ref<E> && false; 

template<class G> concept bool Vertex_ref = true;      // most general and slowest
template<class G> concept bool DAG_vertex_ref = false;  // operations simplified for DAGs (any extra operations?)
template<class G> concept bool Tree_vertex_ref = false; // operations simplified for DAGs (any extra operations?)

// the value type (in a more general design, this would be a template parmeter):
struct Val {};

// specific directed graphs (note: we can't assume common structure or common naming from implementation):
struct Tree {
  	using value_type = Val;
};

struct Node_ref {};
struct Edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

void tops(Tree&);	// return vector Tree_vertex_refs
Node_ref top(Tree&);

struct Dag {
  	using value_type = Val;
};

struct DAG_vertex_ref {};
struct DAG_edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

void tops(Dag&);

struct Dgraph {
  	using value_type = Val;
};


struct Gnode_ref {};
struct Gedge_ref {};	// Is an Edge an object? (if not, refer to parent node)

// another Graph representation:
struct DGN_ref {};
struct DGE_ref {};	// Is an Edge an object? (if not, refer to parent node)

// use:
#include <vector>
using namespace std;

template<Graph G, Vertex_ref R>
void traverse(G& g, R r)
{
	vector<typename G::value_type> found;	// member g::value_type (old habit: could just have used Val)
  	// ... 
}

void use1(Tree& t, Dag& d, Dgraph& dg, Node_ref& tr, DAG_vertex_ref& dr, Gnode_ref& gr)
{
  traverse(t,tr);
  traverse(d,dr);
  traverse(dg,gr);
}

*/


/*
	Basic idea: provide concepts that define the interface(s) to different kinds of graphs so that you can do
	basic graph operations and algoriths without knowing exactly which kind of graph it is and keep ignorant
	about implementation details.
    
    Basic design idea: do like the STL
*/
/*
// Graph concepts (like STL containers):
// Do we need them (STL doesn't make containers explicit)
template<class G> concept bool Graph =  // general graph operations
	requires { typename G::value_type; };
template<class G> concept bool DAG = Graph<G> && requires(G g) { tops(g); };	 // operations simplified for DAGs 
template<class G> concept bool Tree = DAG<G> && requires(G g) { top(g); };	 // operations simplified for trees

// accessor concepts (like STL Iterators):
template<class E> concept bool Edge_ref =      // most general and slowest
	requires(E e) {
  		typename E::value_type;
  		{ *e } -> typename E::value_type;
  		{ e.vertex } -> Vertex_ref;
	};
template<class E> concept bool DAG_edge_ref = // operations simplified for DAGs (any extra operations?)
	Edge_ref<E> && false;	 
template<class E> concept bool Tree_edge_ref = // operations simplified for trees (any extra operations?)
	DAG_edge_ref<E> && false; 

template<class V> concept bool Vertex_ref =       // most general and slowest
  requires(V v, int i) {
  		typename V::value_type;
  		{ *v } -> typename V::value_type;
  		{ v.edge[i] } -> Edge_ref;
	};
template<class V> concept bool DAG_vertex_ref = false;  // operations simplified for DAGs (any extra operations?)
template<class V> concept bool Tree_vertex_ref = false; // operations simplified for DAGs (any extra operations?)

// the value type (in a more general design, this would be a template parmeter):
struct Val {};

// specific directed graphs (note: we can't assume common structure or common naming from implementation):
struct Tree {
  	using value_type = Val;
};

struct Node_ref {};
struct Edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

void tops(Tree&);	// return vector Tree_vertex_refs
Node_ref top(Tree&);

struct Dag {
  	using value_type = Val;
};

struct DAG_vertex_ref {};
struct DAG_edge_ref {};	// Is an Edge an object? (if not, refer to parent node)

void tops(Dag&);

struct Dgraph {
  	using value_type = Val;
};


struct Gnode_ref {};
struct Gedge_ref {};	// Is an Edge an object? (if not, refer to parent node)

// another Graph representation:
struct DGN_ref {};
struct DGE_ref {};	// Is an Edge an object? (if not, refer to parent node)

// use:
#include <vector>
using namespace std;

template<Graph G, Vertex_ref R>
void traverse(G& g, R r)
{
	vector<typename G::value_type> found;	// member g::value_type (old habit: could just have used Val)
  	// ... 
}

void use1(Tree& t, Dag& d, Dgraph& dg, Node_ref& tr, DAG_vertex_ref& dr, Gnode_ref& gr)
{
  traverse(t,tr);
  traverse(d,dr);
  traverse(dg,gr);
}