#include "Mesh.h"

namespace oakvr
{
	namespace render
	{

		// --------------------------------------------------------------------------------
		Mesh::Mesh()
		{
		}

		// --------------------------------------------------------------------------------
		Mesh::~Mesh()
		{	
		}

		void Mesh::AddMeshElement(std::shared_ptr<MeshElement> &meshElem)
		{
			m_vMeshElements.push_back(meshElem);
		}

		//// --------------------------------------------------------------------------------
		//void Mesh::Release()
		//{
		//	/*if(m_pVertexData)
		//	{	
		//		delete[] m_pVertexData;
		//		m_pVertexData = NULL;
		//	}

		//	if(m_pIndexData)
		//	{
		//		delete[] m_pIndexData;
		//		m_pIndexData = NULL;
		//	}
		//	// TODO need pointers here?
		//	for(uint32_t i = 0, n = m_vMeshElements.size(); i < n; ++i)
		//	{	
		//		delete m_vMeshElements[i];
		//		m_vMeshElements[i] = NULL;
		//	}
		//	
		//	m_vMeshElements.clear();
		//*/
		//	SetState(eRS_Released);
		//	
		//}
		
		/*
		// --------------------------------------------------------------------------------
		void Mesh::LoadMaterials()
		{
			m_vMaterials.resize(m_pMeshInfo->m_vMeshElements.size());
			for(uint32_t i = 0, s = m_pMeshInfo->m_vMeshElements.size(); i < s; ++i)
			{
				m_vMaterials[i]= new CMaterial();
				if(m_pMeshInfo->m_vElements[i]->m_path0 != "")
					m_vMaterials[i]->m_texture0 = MakeTexture(m_pMesh->m_vMeshElements[i]->m_path0);
		
				if(m_pMeshInfo->m_vElements[i]->m_path1 != "")
					m_vMaterials[i]->m_texture1 = MakeTexture(m_pMesh->m_vMeshElements[i]->m_path1);
		
				m_vMaterials[i]->m_blendMode = m_pMeshInfo->m_vElements[i]->m_matBlendMode;		
		
			}
			m_bMaterialsInitialised = true;
		}

		// --------------------------------------------------------------------------------
		void Mesh::Init(const Path &path)
		{
			m_pMeshInfo = MakeMeshInfo(path);
			m_bMaterialsInitialised = false;
		}
		
		
		// --------------------------------------------------------------------------------
		void Mesh::SetNumVertices(unsigned int numVertices)
		{
			m_numVertices = numVertices;
		}

		// --------------------------------------------------------------------------------
		void Mesh::SetHasNormals(bool bHasNormals)
		{
			m_bHasNormals = bHasNormals;
		}

		// --------------------------------------------------------------------------------
		void Mesh::SetNumTexCoordsPerVertex(unsigned int numTexCoordsPerVertex)
		{
			m_numTexCoordsPerVertex = numTexCoordsPerVertex;
		}

		// --------------------------------------------------------------------------------
		void Mesh::SetNumMaterials(unsigned int numMaterials)
		{
			m_numMaterials = numMaterials;
		}*/

	} // namespace render
}// namespace oakvr
