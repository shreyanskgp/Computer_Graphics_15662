#include <float.h>
#include <assert.h>
#include "meshEdit.h"
#include "mutablePriorityQueue.h"

namespace CMU462 {

	VertexIter HalfedgeMesh::splitEdge(EdgeIter e0) {

		// TODO: (meshEdit)
		// This method should split the given edge and return an iterator to the
		// newly inserted vertex. The halfedge of this vertex should point along
		// the edge that was split, rather than the new edges.



		//Check if the edge has two triangles associated with it

		if (e0->halfedge()->face()->degree() != 3 || e0->halfedge()->twin()->face()->degree() != 3) return e0->halfedge()->vertex();

		HalfedgeIter he_edge = e0->halfedge();


		VertexIter new_vertex = newVertex();

		vector<HalfedgeIter> new_halfedges;
		vector<EdgeIter> new_edges;
		vector<FaceIter> new_faces;

		for (int i = 0; i < 6; i++)
		{
			new_halfedges.push_back(newHalfedge());
		}
		for (int i = 0; i < 3; i++)
		{
			new_edges.push_back(newEdge());
		}
		for (int i = 0; i < 2; i++)
		{
			new_faces.push_back(newFace());
		}


		//New vertex position
		new_vertex->position = e0->centroid();

		//Reassigning old faces halfedges
		he_edge->face()->halfedge() = he_edge;
		he_edge->twin()->face()->halfedge() = he_edge->twin();


		he_edge->next()->face() = he_edge->face();
		he_edge->next()->next()->face() = new_faces[0];
		he_edge->twin()->next()->face() = new_faces[1];
		he_edge->twin()->next()->next()->face() = he_edge->twin()->face();

		//Old vertex halfedge

		he_edge->vertex()->halfedge() = he_edge->twin()->next();

		//Assigning 1 new vertex halfedge
		new_vertex->halfedge() = he_edge;

		//Assigning halfedges to 3 new edges
		new_edges[0]->halfedge() = new_halfedges[0];
		new_edges[1]->halfedge() = new_halfedges[2];
		new_edges[2]->halfedge() = new_halfedges[4];

		//Assigning haledges to 2 new faces
		new_faces[0]->halfedge() = new_halfedges[1];
		new_faces[1]->halfedge() = new_halfedges[4];

		//Assigning neighbors to 6 new halfedges
		//setNeighbors(next,twin,vertex,edge,face)
		new_halfedges[0]->setNeighbors(he_edge, new_halfedges[1], he_edge->next()->next()->vertex(), new_edges[0], he_edge->face());

		new_halfedges[1]->setNeighbors(he_edge->next()->next(), new_halfedges[0], new_vertex, new_edges[0], new_faces[0]);

		new_halfedges[2]->setNeighbors(new_halfedges[1], new_halfedges[3], he_edge->vertex(), new_edges[1], new_faces[0]);

		new_halfedges[3]->setNeighbors(he_edge->twin()->next(), new_halfedges[2], new_vertex, new_edges[1], new_faces[1]);

		new_halfedges[4]->setNeighbors(new_halfedges[3], new_halfedges[5], he_edge->twin()->next()->next()->vertex(), new_edges[2], new_faces[1]);

		new_halfedges[5]->setNeighbors(he_edge->twin()->next()->next(), new_halfedges[4], new_vertex, new_edges[2], he_edge->twin()->face());

		//Reassignin next of twin of he_edge

		//Reconnecting old halfedges
		he_edge->next()->next()->next() = new_halfedges[2];
		he_edge->next()->next() = new_halfedges[0];

		he_edge->twin()->next()->next() = new_halfedges[4];
		he_edge->twin()->next() = new_halfedges[5];
		he_edge->vertex() = new_vertex;

		return new_vertex;
	}

	VertexIter HalfedgeMesh::collapseEdge(EdgeIter e) {

		// TODO: (meshEdit)
		// This method should collapse the given edge and return an iterator to
		// the new vertex created by the collapse.
		return VertexIter();
	}

	VertexIter HalfedgeMesh::collapseFace(FaceIter f) {

		// TODO: (meshEdit)
		// This method should collapse the given face and return an iterator to
		// the new vertex created by the collapse.
		return VertexIter();
	}

	FaceIter HalfedgeMesh::eraseVertex(VertexIter v) {

		// TODO: (meshEdit)
		// This method should replace the given vertex and all its neighboring
		// edges and faces with a single face, returning the new face.


		//Boundary vertex is not erased
		if (v->isBoundary())
		{
			return v->halfedge()->face();
		}
		VertexIter temp_v = v;

		FaceIter temp_f = newFace();
		HalfedgeIter temp_h = v->halfedge();
		HalfedgeIter temp_h_iter = temp_h;

		//Assigning all halfedges to the new face
		do {

			deleteFace(temp_h_iter->face());
			do {

				temp_h_iter->face() = temp_f;
				temp_h_iter = temp_h_iter->next();
			} while (temp_h_iter != temp_h);

			//Reassign vertex halfedge if required
			if (temp_h->twin()->vertex()->halfedge() == temp_h->twin())
			{
				temp_h->twin()->vertex()->halfedge() = temp_h->next();
			}


			temp_h = temp_h->twin()->next();
			temp_h_iter = temp_h;
		} while (v->halfedge() != temp_h);

		temp_f->halfedge() = temp_h->next();


		//Reassigning half-edges for connectivity of new mesh
		temp_h = v->halfedge();
		temp_h_iter = temp_h->twin();

		do {
			do {
				temp_h_iter = temp_h_iter->next();
			} while (temp_h_iter->next()->twin() != temp_h);

			temp_h_iter->next() = temp_h->next();

			temp_h = temp_h->twin()->next();
			temp_h_iter = temp_h->twin();
		} while (v->halfedge() != temp_h);


		//Deleting edges
		temp_h = v->halfedge();
		temp_h_iter = temp_h;
		HalfedgeIter temp_h_prev_twin;

		do {
			if (temp_h->twin()->next() == v->halfedge())
			{
				temp_h_prev_twin = temp_h;
			}

			deleteEdge(temp_h->edge());
			temp_h = temp_h->twin()->next();

		} while (v->halfedge() != temp_h);



		//Deleting half-edges
		temp_h = v->halfedge();
		temp_h_iter = temp_h;
		do {
			temp_h_iter = temp_h;
			temp_h = temp_h->twin()->next();
			deleteHalfedge(temp_h_iter->twin());
			deleteHalfedge(temp_h_iter);
		} while (temp_h_prev_twin != temp_h);
		deleteHalfedge(temp_h->twin());
		deleteHalfedge(temp_h);

		//Deleting vertex
		deleteVertex(temp_v);

		return temp_f;
	}

	FaceIter HalfedgeMesh::eraseEdge(EdgeIter e) {
		// TODO: (meshEdit)
		// This method should erase the given edge and return an iterator to the
		// merged face.
		FaceIter temp_f = newFace();

		EdgeIter temp_e = e;



		return FaceIter();
	}

	EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0) {

		// TODO: (meshEdit)
		// This method should flip the given edge and return an iterator to the
		// flipped edge.

		if (e0->isBoundary())
		{
			return e0;
		}
		HalfedgeIter temp_he = e0->halfedge();


		//Loop through halfedge
		while (temp_he->next() != e0->halfedge())
		{
			temp_he = temp_he->next();

		}

		HalfedgeIter temp_he_twin = e0->halfedge()->twin();

		//Loop through twin of halfedge    
		while (temp_he_twin->next() != e0->halfedge()->twin())
		{
			temp_he_twin = temp_he_twin->next();

		}

		if (e0->halfedge()->face()->halfedge() == e0->halfedge()->next())
		{
			e0->halfedge()->face()->halfedge() = e0->halfedge();

		}

		if (e0->halfedge()->twin()->face()->halfedge() == e0->halfedge()->twin()->next())
		{
			e0->halfedge()->twin()->face()->halfedge() = e0->halfedge()->twin();

		}

		//Assigning the halfedges of the vertices
		e0->halfedge()->vertex()->halfedge() = e0->halfedge()->twin()->next();
		e0->halfedge()->twin()->vertex()->halfedge() = e0->halfedge()->next();

		//Exchanging faces of new half edges
		e0->halfedge()->next()->face() = e0->halfedge()->twin()->face();
		e0->halfedge()->twin()->next()->face() = e0->halfedge()->face();

		//Assign new vertices to the half edges
		e0->halfedge()->vertex() = e0->halfedge()->twin()->next()->next()->vertex();
		e0->halfedge()->twin()->vertex() = e0->halfedge()->next()->next()->vertex();

		//Reassign previous half edges to point to new half edges from other side
		temp_he->next() = e0->halfedge()->twin()->next();
		temp_he_twin->next() = e0->halfedge()->next();

		//Assign next of e0 half edge and twin
		e0->halfedge()->next() = temp_he_twin->next()->next();
		e0->halfedge()->twin()->next() = temp_he->next()->next();


		//Assign e0 half edge and twin as next of new half edges from other side
		temp_he->next()->next() = e0->halfedge();
		temp_he_twin->next()->next() = e0->halfedge()->twin();

		return e0;

	}

	void HalfedgeMesh::subdivideQuad(bool useCatmullClark)
	{
		// Unlike the local mesh operations (like bevel or edge flip), we will perform
		// subdivision by splitting *all* faces into quads "simultaneously."  Rather
		// than operating directly on the halfedge data structure (which as you've seen
		// is quite difficult to maintain!) we are going to do something a bit nicer:
		//
		//    1. Create a raw list of vertex positions and faces (rather than a full-
		//       blown halfedge mesh).
		//
		//    2. Build a new halfedge mesh from these lists, replacing the old one.
		//
		// Sometimes rebuilding a data structure from scratch is simpler (and even more
		// efficient) than incrementally modifying the existing one.  These steps are
		// detailed below.

		// TODO Step I: Compute the vertex positions for the subdivided mesh.  Here we're
		// going to do something a little bit strange: since we will have one vertex in
		// the subdivided mesh for each vertex, edge, and face in the original mesh, we
		// can nicely store the new vertex *positions* as attributes on vertices, edges,
		// and faces of the original mesh.  These positions can then be conveniently
		// copied into the new, subdivided mesh.
		// [See subroutines for actual "TODO"s]
		if (useCatmullClark)
		{
			computeCatmullClarkPositions();
		}
		else
		{
			computeLinearSubdivisionPositions();
		}

		// TODO Step II: Assign a unique index (starting at 0) to each vertex, edge, and
		// face in the original mesh.  These indices will be the indices of the vertices
		// in the new (subdivided mesh).  They do not have to be assigned in any particular
		// order, so long as no index is shared by more than one mesh element, and the
		// total number of indices is equal to V+E+F, i.e., the total number of vertices
		// plus edges plus faces in the original mesh.  Basically we just need a one-to-one
		// mapping between original mesh elements and subdivided mesh vertices.
		// [See subroutine for actual "TODO"s]
		assignSubdivisionIndices();

		// TODO Step III: Build a list of quads in the new (subdivided) mesh, as tuples of
		// the element indices defined above.  In other words, each new quad should be of
		// the form (i,j,k,l), where i,j,k and l are four of the indices stored on our
		// original mesh elements.  Note that it is essential to get the orientation right
		// here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces should
		// circulate in the same direction as old faces (think about the right-hand rule).
		// [See subroutines for actual "TODO"s]
		vector< vector<Index> > subDFaces;
		vector< Vector3D > subDVertices;
		buildSubdivisionFaceList(subDFaces);
		buildSubdivisionVertexList(subDVertices);

		// TODO Step IV: Pass the list of vertices and quads to a routine that clears the
		// internal data for this halfedge mesh, and builds new halfedge data from scratch,
		// using the two lists.
		rebuild(subDFaces, subDVertices);
	}

	/**
	* Compute new vertex positions for a mesh that splits each polygon
	* into quads (by inserting a vertex at the face midpoint and each
	* of the edge midpoints).  The new vertex positions will be stored
	* in the members Vertex::newPosition, Edge::newPosition, and
	* Face::newPosition.  The values of the positions are based on
	* simple linear interpolation, e.g., the edge midpoints and face
	* centroids.
	*/
	void HalfedgeMesh::computeLinearSubdivisionPositions()
	{
		// TODO For each vertex, assign Vertex::newPosition to
		// its original position, Vertex::position.

		// TODO For each edge, assign the midpoint of the two original
		// positions to Edge::newPosition.

		// TODO For each face, assign the centroid (i.e., arithmetic mean)
		// of the original vertex positions to Face::newPosition.  Note
		// that in general, NOT all faces will be triangles!


		for (VertexIter v = verticesBegin(); v != verticesEnd(); ++v)
		{
			v->newPosition = v->position;
		}


		for (EdgeIter e = edgesBegin(); e != edgesEnd(); ++e)
		{
			e->newPosition = e->centroid();
		}


		for (FaceIter f = facesBegin(); f != facesEnd(); ++f)
		{
			f->newPosition = f->centroid();
		}


	}

	/**
	* Compute new vertex positions for a mesh that splits each polygon
	* into quads (by inserting a vertex at the face midpoint and each
	* of the edge midpoints).  The new vertex positions will be stored
	* in the members Vertex::newPosition, Edge::newPosition, and
	* Face::newPosition.  The values of the positions are based on
	* the Catmull-Clark rules for subdivision.
	*/
	void HalfedgeMesh::computeCatmullClarkPositions()
	{
		// TODO The implementation for this routine should be
		// a lot like HalfedgeMesh::computeLinearSubdivisionPositions(),
		// except that the calculation of the positions themsevles is
		// slightly more involved, using the Catmull-Clark subdivision
		// rules.  (These rules are outlined in the Developer Manual.)

		// TODO face

		// TODO edges

		// TODO vertices



		for (FaceIter f = facesBegin(); f != facesEnd(); ++f)
		{
			f->newPosition = f->centroid();
		}

		for (EdgeIter e = edgesBegin(); e != edgesEnd(); ++e)
		{
			e->newPosition = 0.25*(e->centroid() * 2 + e->halfedge()->face()->newPosition + e->halfedge()->twin()->face()->newPosition);
		}

		for (VertexIter v = verticesBegin(); v != verticesEnd(); ++v)
		{
			Vector3D temp_faces_centroid = (0, 0, 0);
			Vector3D temp_edges_centroid = (0, 0, 0);
			HalfedgeIter temp_he;
			temp_he = v->halfedge();
			do {
				temp_he = temp_he->twin()->next();
				temp_faces_centroid = temp_faces_centroid + temp_he->face()->newPosition;
				temp_edges_centroid = temp_edges_centroid + temp_he->edge()->newPosition;
			} while (temp_he != v->halfedge());

			temp_faces_centroid = temp_faces_centroid / v->degree();
			temp_edges_centroid = temp_edges_centroid / v->degree();

			v->newPosition = (temp_faces_centroid + 2 * temp_edges_centroid + v->position*(v->degree() - 3)) / (v->degree());
		}




	}

	/**
	* Assign a unique integer index to each vertex, edge, and face in
	* the mesh, starting at 0 and incrementing by 1 for each element.
	* These indices will be used as the vertex indices for a mesh
	* subdivided using Catmull-Clark (or linear) subdivision.
	*/
	void HalfedgeMesh::assignSubdivisionIndices()
	{
		// TODO Start a counter at zero; if you like, you can use the
		// "Index" type (defined in halfedgeMesh.h)

		// TODO Iterate over vertices, assigning values to Vertex::index

		// TODO Iterate over edges, assigning values to Edge::index

		// TODO Iterate over faces, assigning values to Face::index

		Index i = 0;
		for (VertexIter v = verticesBegin(); v != verticesEnd(); ++v)
		{
			v->index = i;
			i++;
		}


		for (EdgeIter e = edgesBegin(); e != edgesEnd(); ++e)
		{
			e->index = i;
			i++;
		}


		for (FaceIter f = facesBegin(); f != facesEnd(); ++f)
		{
			f->index = i;
			i++;
		}
	}

	/**
	* Build a flat list containing all the vertex positions for a
	* Catmull-Clark (or linear) subdivison of this mesh.  The order of
	* vertex positions in this list must be identical to the order
	* of indices assigned to Vertex::newPosition, Edge::newPosition,
	* and Face::newPosition.
	*/
	void HalfedgeMesh::buildSubdivisionVertexList(vector<Vector3D>& subDVertices)
	{
		// TODO Resize the vertex list so that it can hold all the vertices.

		// TODO Iterate over vertices, assigning Vertex::newPosition to the appropriate
		// location in the new vertex list.

		// TODO Iterate over edges, assigning Edge::newPosition to the appropriate
		// location in the new vertex list.

		// TODO Iterate over faces, assigning Face::newPosition to the appropriate
		// location in the new vertex list.

		for (VertexIter v = verticesBegin(); v != verticesEnd(); ++v)
		{
			subDVertices.push_back(v->newPosition);

		}


		for (EdgeIter e = edgesBegin(); e != edgesEnd(); ++e)
		{
			subDVertices.push_back(e->newPosition);
		}


		for (FaceIter f = facesBegin(); f != facesEnd(); ++f)
		{
			subDVertices.push_back(f->newPosition);
		}


	}

	/**
	* Build a flat list containing all the quads in a Catmull-Clark
	* (or linear) subdivision of this mesh.  Each quad is specified
	* by a vector of four indices (i,j,k,l), which come from the
	* members Vertex::index, Edge::index, and Face::index.  Note that
	* the ordering of these indices is important because it determines
	* the orientation of the new quads; it is also important to avoid
	* "bowties."  For instance, (l,k,j,i) has the opposite orientation
	* of (i,j,k,l), and if (i,j,k,l) is a proper quad, then (i,k,j,l)
	* will look like a bowtie.
	*/
	void HalfedgeMesh::buildSubdivisionFaceList(vector< vector<Index> >& subDFaces)
	{
		// TODO This routine is perhaps the most tricky step in the construction of
		// a subdivision mesh (second, perhaps, to computing the actual Catmull-Clark
		// vertex positions).  Basically what you want to do is iterate over faces,
		// then for each for each face, append N quads to the list (where N is the
		// degree of the face).  For this routine, it may be more convenient to simply
		// append quads to the end of the list (rather than allocating it ahead of
		// time), though YMMV.  You can of course iterate around a face by starting
		// with its first halfedge and following the "next" pointer until you get
		// back to the beginning.  The tricky part is making sure you grab the right
		// indices in the right order---remember that there are indices on vertices,
		// edges, AND faces of the original mesh.  All of these should get used.  Also
		// remember that you must have FOUR indices per face, since you are making a
		// QUAD mesh!

		// TODO iterate over faces
		// TODO loop around face
		// TODO build lists of four indices for each sub-quad
		// TODO append each list of four indices to face list

		vector<Index> quad(4);
		size_t num_sides;
		HalfedgeIter temp_he, temp_he_prev;

		for (FaceIter f = facesBegin(); f != facesEnd(); ++f)
		{

			num_sides = f->degree();
			temp_he = f->halfedge();
			temp_he_prev = temp_he;

			do {
				temp_he_prev = temp_he_prev->next();

			} while (temp_he_prev->next() != temp_he);

			for (size_t i = 0; i<num_sides; i++)
			{
				quad[0] = temp_he->vertex()->index;
				quad[1] = temp_he->edge()->index;
				quad[2] = temp_he->face()->index;
				quad[3] = temp_he_prev->edge()->index;

				subDFaces.push_back(quad);
				temp_he_prev = temp_he;
				temp_he = temp_he->next();
			}
		}

	}

	void HalfedgeMesh::_bevel_fc_reposition_with_dist(vector<Vector3D>& orig, // list of vertex positions of the original face (before bevel)
		vector<HalfedgeIter>& hs, // list of halfedges pointing from the vertices of the new, beveled face to the old, original face
		double shift, // user-requested amount to shift the face in the normal direction
		double inset) // user-requested amount by which to inset (i.e., grow/shrink) the beveled face
	{
		// TODO Compute new vertex positions for the vertices of the beveled face.
		//
		// These vertices can be accessed via hs[i]->vertex()->position for i = 1, ..., hs.size()-1.
		//
		// The basic strategy here is to loop over the list of outgoing halfedges,
		// and use the preceding and next vertex position from the original mesh
		// (in the orig array) to compute an offset vertex position.
		//
		// Note that there is a 1-to-1 correspondence between halfedges in hs and vertex positions
		// in orig.  So, you can write loops of the form
		//
		// for( int i = 0; i < hs.size(); hs++ )
		// {
		//    Vector3D pi = orig[i]; // get the original vertex position correponding to vertex i
		// }
		//

		//Getting the beveled face
		FaceIter temp_f = hs[0]->twin()->next()->twin()->face();

		Vector3D unit_face_normal = temp_f->normal();
		Vector3D face_centroid = temp_f->centroid();


		for (int i = 0; i < hs.size(); i++)
		{
			Vector3D pi = orig[i]; // get the original vertex position correponding to vertex i


			hs[i]->vertex()->position = hs[i]->vertex()->position + unit_face_normal*shift + (pi - face_centroid)*(inset);

		}


	}

	void HalfedgeMesh::_bevel_vtx_reposition_with_dist(Vector3D orig, // original vertex position, before the bevel
		vector<HalfedgeIter>& hs, // list of halfedges pointing from the vertices of the new, beveled face to the neighbors of the original vertex
		double inset) // user-requested amount by which to inset (i.e., grow/shrink) the beveled face
	{
		// TODO Compute new vertex positions for the vertices of the beveled vertex.
		//
		// These vertices can be accessed via hs[i]->vertex()->position for i = 1, ..., hs.size()-1.
		//
		// The basic strategy here is to loop over the list of outgoing halfedges,
		// and use the preceding and next vertex position from the original mesh
		// (in the orig array) to compute an offset vertex position.
		//
		// Note that there is a 1-to-1 correspondence between halfedges in hs and vertex positions
		// in orig.  So, you can write loops of the form
		//
		// for( int i = 0; i < hs.size(); hs++ )
		// {
		//    Vector3D pi = orig[i]; // get the original vertex position correponding to vertex i
		// }
		//
	}

	void HalfedgeMesh::_bevel_edge_reposition_with_dist(vector<Vector3D>& origs,  // list of vertex positions of the neighbors of the two endpoints of the edge, before the bevel
		vector<HalfedgeIter>& hs,  // list of halfedges pointing from the vertices of the new, beveled face to the neighbors of the endpoints of the old, original edge
		double inset) // user-requested amount by which to inset (i.e., grow/shrink) the beveled face
	{
		// TODO Compute new vertex positions for the vertices of the beveled edge.
		//
		// These vertices can be accessed via hs[i]->vertex()->position for i = 1, ..., hs.size()-1.
		//
		// The basic strategy here is to loop over the list of outgoing halfedges,
		// and use the preceding and next vertex position from the original mesh
		// (in the orig array) to compute an offset vertex position.
		//
		// Note that there is a 1-to-1 correspondence between halfedges in hs and vertex positions
		// in orig.  So, you can write loops of the form
		//
		// for( int i = 0; i < hs.size(); hs++ )
		// {
		//    Vector3D pi = orig[i]; // get the original vertex position correponding to vertex i
		// }
		//
	}

	FaceIter HalfedgeMesh::bevelVertex(VertexIter v) {

		// TODO This method should replace the vertex v with a face, corresponding to a bevel operation.
		// It should return the new face.  NOTE: This method is responsible for updating the *connectivity*
		// of the mesh only---it does not need to update the vertex positions.  These positions will be
		// updated in HalfedgeMesh::_bevel_vtx_reposition_with_dist (which you also have to implement!)

		return facesBegin();
	}

	FaceIter HalfedgeMesh::bevelEdge(EdgeIter e) {

		// TODO This method should replace the edge e with a face, corresponding to a bevel operation.
		// It should return the new face.  NOTE: This method is responsible for updating the *connectivity*
		// of the mesh only---it does not need to update the vertex positions.  These positions will be
		// updated in HalfedgeMesh::_bevel_vtx_reposition_with_dist (which you also have to implement!)

		return facesBegin();
	}

	FaceIter HalfedgeMesh::bevelFace(FaceIter f) {

		// TODO This method should replace the face f with an additional, inset face (and ring of faces around it),
		// corresponding to a bevel operation. It should return the new face.  NOTE: This method is responsible for
		// updating the *connectivity* of the mesh only---it does not need to update the vertex positions.  These
		// positions will be updated in HalfedgeMesh::_bevel_vtx_reposition_with_dist (which you also have to
		// implement!)
		if (f->isBoundary()) return f;

		size_t num_sides = f->degree();

		vector<VertexIter> new_vertices;
		vector<FaceIter> new_faces;
		vector<HalfedgeIter> new_halfedges;
		vector<EdgeIter> new_edges;
		HalfedgeIter face_he = f->halfedge();
		HalfedgeIter temp_he;

		//Adding new elements to the corresponding vectors

		for (size_t i = 0; i<num_sides; i++)
		{
			new_vertices.push_back(newVertex());
			new_faces.push_back(newFace());
			new_halfedges.push_back(newHalfedge());
			new_halfedges.push_back(newHalfedge());
			new_halfedges.push_back(newHalfedge());
			new_halfedges.push_back(newHalfedge());
			new_edges.push_back(newEdge());
			new_edges.push_back(newEdge());
		}

		//
		for (size_t i = 0; i<num_sides; i++)
		{

			//ORIGINAL ELEMENTS REASSIGNMENT
			//New face to the original half edge
			face_he->face() = new_faces[i];

			//New halfedge to the original vertex
			face_he->next()->vertex()->halfedge() = new_halfedges[4 * i];

			//NEW ELEMENTS ASSIGNMENT
			//Assigning halfedge to 1 new face
			new_faces[i]->halfedge() = new_halfedges[4 * i + 2];

			//Assigning halfedges to 2 new edges
			new_edges[2 * i]->halfedge() = new_halfedges[4 * i + 1];
			new_edges[2 * i + 1]->halfedge() = new_halfedges[4 * i + 2];

			//Assigning halfedge to 1 new vertex
			new_vertices[i]->halfedge() = new_halfedges[4 * i + 1];

			//Assigning neighbors to 4 new halfedges
			//setNeighbors(next,twin,vertex,edge,face)
			new_halfedges[4 * i]->setNeighbors(new_halfedges[4 * i + 2], new_halfedges[4 * i + 1], face_he->next()->vertex(), new_edges[2 * i], new_faces[i]);
			new_halfedges[4 * i + 1]->setNeighbors(face_he->next(), new_halfedges[4 * i], new_vertices[i], new_edges[2 * i], new_faces[(i + 1) % num_sides]);
			new_halfedges[4 * i + 2]->setNeighbors(new_halfedges[4 * ((i + num_sides - 1) % num_sides) + 1], new_halfedges[4 * i + 3], new_vertices[i], new_edges[2 * i + 1], new_faces[i]);
			new_halfedges[4 * i + 3]->setNeighbors(new_halfedges[4 * ((i + 1) % num_sides) + 3], new_halfedges[4 * i + 2], new_vertices[(i + num_sides - 1) % num_sides], new_edges[2 * i + 1], f);

			//Assigning vertex position to the new vertex
			new_vertices[i]->position = face_he->next()->vertex()->position;

			temp_he = face_he;
			face_he = face_he->next();

			//Reassigning next of original halfedge
			temp_he->next() = new_halfedges[4 * i];
		}

		f->halfedge() = new_halfedges[3];


		return f;
		//return facesBegin();
	}

	void HalfedgeMesh::splitPolygons(vector<FaceIter>& fcs) {
		for (auto f : fcs) splitPolygon(f);
	}

	void HalfedgeMesh::splitPolygon(FaceIter f) {
		// TODO triangulation

		size_t num_sides = f->degree();

		HalfedgeIter f_ori_he = f->halfedge();
		HalfedgeIter temp_he = f_ori_he;


		if (num_sides == 3) return;


		for (size_t i = 0; i<(num_sides - 3); i++)
		{
			HalfedgeIter new_he = newHalfedge();
			HalfedgeIter new_he_twin = newHalfedge();
			EdgeIter new_edge = newEdge();
			FaceIter new_face;

			if (i == 0) //Assigning original face to the first triangle
			{
				new_face = f;
			}
			else
			{
				new_face = newFace();
			}

			new_edge->halfedge() = new_he;


			//Assigning face to the halfedges
			temp_he->face() = new_face;
			temp_he->next()->face() = new_face;

			//Assigning half-edge to face
			new_face->halfedge() = temp_he;

			//New halfedge assignments of mesh elements
			new_he->setNeighbors(temp_he, new_he_twin, temp_he->next()->next()->vertex(), new_edge, new_face);

			//Assigning properties to twin
			new_he_twin->edge() = new_edge;
			new_he_twin->twin() = new_he;
			new_he_twin->vertex() = f_ori_he->vertex();
			new_he_twin->next() = temp_he->next()->next();

			//Changing connectivity of old half edges
			temp_he->next()->next() = new_he;

			temp_he = new_he_twin;
		}

		//Assigning face and next elements for the last face
		FaceIter new_face = newFace();

		temp_he->face() = new_face;
		temp_he->next()->face() = new_face;
		temp_he->next()->next()->face() = new_face;
		temp_he->next()->next()->next() = temp_he;
		new_face->halfedge() = temp_he;



	}

	EdgeRecord::EdgeRecord(EdgeIter& _edge) : edge(_edge) {

		// TODO: (meshEdit)
		// Compute the combined quadric from the edge endpoints.
		// -> Build the 3x3 linear system whose solution minimizes the quadric error
		//    associated with these two endpoints.
		// -> Use this system to solve for the optimal position, and store it in
		//    EdgeRecord::optimalPoint.
		// -> Also store the cost associated with collapsing this edg in
		//    EdgeRecord::Cost.

	}

	void MeshResampler::upsample(HalfedgeMesh& mesh)
		// This routine should increase the number of triangles in the mesh using Loop subdivision.
	{

		// TODO: (meshEdit)
		// Compute new positions for all the vertices in the input mesh, using
		// the Loop subdivision rule, and store them in Vertex::newPosition.
		// -> At this point, we also want to mark each vertex as being a vertex of the
		//    original mesh.
		// -> Next, compute the updated vertex positions associated with edges, and
		//    store it in Edge::newPosition.
		// -> Next, we're going to split every edge in the mesh, in any order.  For
		//    future reference, we're also going to store some information about which
		//    subdivided edges come from splitting an edge in the original mesh, and
		//    which edges are new, by setting the flat Edge::isNew. Note that in this
		//    loop, we only want to iterate over edges of the original mesh.
		//    Otherwise, we'll end up splitting edges that we just split (and the
		//    loop will never end!)
		// -> Now flip any new edge that connects an old and new vertex.
		// -> Finally, copy the new vertex positions into final Vertex::position.

		// Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
		// Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
		// using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
		// the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
		// the new mesh based on the values we computed for the original mesh.

		// Compute updated positions for all the vertices in the original mesh, using the Loop subdivision rule.

		// Next, compute the updated vertex positions associated with edges.

		// Next, we're going to split every edge in the mesh, in any order.  For future
		// reference, we're also going to store some information about which subdivided
		// edges come from splitting an edge in the original mesh, and which edges are new.
		// In this loop, we only want to iterate over edges of the original mesh---otherwise,
		// we'll end up splitting edges that we just split (and the loop will never end!)

		// Finally, flip any new edge that connects an old and new vertex.

		// Copy the updated vertex positions to the subdivided mesh.



		//Saving the position of original vertices in newPosition

		for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); ++v)
		{

			double u;
			double n = (double)(v->degree());

			if (v->degree() == 3)
			{
				u = 3. / 16.;
			}
			else
			{
				u = 3. / (8.*n);
			}



			v->newPosition = (1. - n*u)*v->position + (u*n)*v->neighborhoodCentroid();

			v->isNew = false;
		}

		//Saving the positions of new vertices in the edges newPosition
		for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); ++e)
		{
			e->newPosition = (3. / 8.)*(e->halfedge()->vertex()->position + e->halfedge()->twin()->vertex()->position) + (1. / 8.)* (e->halfedge()->next()->next()->vertex()->position + e->halfedge()->twin()->next()->next()->vertex()->position);
			e->isNew = false;
		}

		VertexIter v_temp;
		EdgeIter e_end = mesh.edgesEnd();

		HalfedgeIter he_temp;
		Vector3D e_new_pos_hold;
		vector<EdgeIter> original_edges;


		EdgeIter e_next;
		EdgeIter e = mesh.edgesBegin();


		while (e != mesh.edgesEnd())
		{
			e_next = e;
			e_next++;
			if (e->isNew == false && e->isBoundary() == false)
			{
				e_new_pos_hold = e->newPosition;
				v_temp = mesh.splitEdge(e);
				v_temp->newPosition = e->newPosition;
				v_temp->isNew = true;

				he_temp = v_temp->halfedge();

				do {
					he_temp->edge()->isNew = true;
					he_temp = he_temp->twin()->next();

				} while (he_temp != v_temp->halfedge());
				original_edges.push_back(v_temp->halfedge()->edge());
				original_edges.push_back(v_temp->halfedge()->twin()->next()->twin()->next()->edge());
			}
			e = e_next;
		}


		//Flagging the original edges
		for (int i = 0; i<original_edges.size(); i++)
		{
			original_edges[i]->isNew = false;
		}

		int temp_random = 0;

		//Flipping edges having one old and one new vertex
		EdgeIter e_temp;
		e = mesh.edgesBegin();
		while (e != mesh.edgesEnd())
		{
			e_temp = e;
			e_temp++;
			if ((e->isNew == true) && ((e->halfedge()->vertex()->isNew == true && e->halfedge()->twin()->vertex()->isNew == false) || (e->halfedge()->vertex()->isNew == false && e->halfedge()->twin()->vertex()->isNew == true)))
			{
				e_temp = mesh.flipEdge(e);

			}
			e = e_temp;
		}

		for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); ++v)
		{
			v->position = v->newPosition;
		}

	}

	void MeshResampler::downsample(HalfedgeMesh& mesh)
	{

		// TODO: (meshEdit)
		// Compute initial quadrics for each face by simply writing the plane equation
		// for the face in homogeneous coordinates. These quadrics should be stored
		// in Face::quadric
		// -> Compute an initial quadric for each vertex as the sum of the quadrics
		//    associated with the incident faces, storing it in Vertex::quadric
		// -> Build a priority queue of edges according to their quadric error cost,
		//    i.e., by building an EdgeRecord for each edge and sticking it in the
		//    queue.
		// -> Until we reach the target edge budget, collapse the best edge. Remember
		//    to remove from the queue any edge that touches the collapsing edge
		//    BEFORE it gets collapsed, and add back into the queue any edge touching
		//    the collapsed vertex AFTER it's been collapsed. Also remember to assign
		//    a quadric to the collapsed vertex, and to pop the collapsed edge off the
		//    top of the queue.

	}

	void MeshResampler::resample(HalfedgeMesh& mesh) {

		// TODO: (meshEdit)
		// Compute the mean edge length.
		// Repeat the four main steps for 5 or 6 iterations
		// -> Split edges much longer than the target length (being careful about
		//    how the loop is written!)
		// -> Collapse edges much shorter than the target length.  Here we need to
		//    be EXTRA careful about advancing the loop, because many edges may have
		//    been destroyed by a collapse (which ones?)
		// -> Now flip each edge if it improves vertex degree
		// -> Finally, apply some tangential smoothing to the vertex positions

	}

} // namespace CMU462
