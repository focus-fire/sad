#include "sadpch.h"

#include <filesystem>

#include "RenderableObject.h"

sad::RenderableObject::RenderableObject(RenderableResource* resource)
	: m_RenderableResource(resource)
{
	const RenderableResource::Geometry& geometry = m_RenderableResource->GetGeometry();

	// Create vertex array and buffer  
	m_VertexArray = new sad::rad::VertexArray();
	m_VertexBuffer = new sad::rad::VertexBuffer(geometry.Vertices, geometry.VertexSize); 
	m_VertexAttributes = new sad::rad::VertexAttributeContainer();

	// Stripe the data in 'Vertices' appropriately
	m_VertexAttributes->AddFloatAttribute(3);		// Points, Vec3 
	m_VertexAttributes->AddFloatAttribute(2);		// TextureCoordinates, Vec2
	m_VertexArray->AddBufferWithAttributes(*m_VertexBuffer, *m_VertexAttributes);

	// Create the index buffer for indices
	m_IndexBuffer = new sad::rad::IndexBuffer(geometry.Indices, geometry.IndexCount);

	// Platform specific path resolution
	// TODO: Create method to generate paths for each platform
	std::string dataDirectory = std::filesystem::current_path().string();
	std::string shaderFile, textureFile;

#ifdef _SAD_WINDOWS
	// By default current_path loads from the 'Code' directory on Windows
	std::size_t codeDirectory = dataDirectory.find_last_of("/\\");
	dataDirectory = dataDirectory.substr(0, codeDirectory);

	dataDirectory += "\\Data";
	shaderFile = dataDirectory + "\\Shaders\\Default.glsl";
	textureFile = dataDirectory + "\\Textures\\Default.png";
#else
	dataDirectory += "/Data";
	shaderFile = dataDirectory + "/Shaders/Default.glsl";
	textureFile = dataDirectory + "/Textures/Default.png";
#endif

	// All RenderableObjects start with this 'Material' by default
	m_Shader = new sad::rad::Shader(shaderFile);
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	m_Texture = new sad::rad::Texture(textureFile);
	m_Texture->Bind(1);
	m_Shader->SetUniform1i("u_Texture", 1);
}

sad::RenderableObject::~RenderableObject()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_VertexAttributes;
	delete m_IndexBuffer;
	delete m_Shader;
	delete m_Texture;
}