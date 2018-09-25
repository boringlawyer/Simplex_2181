#include "MyMesh.h"
void MyMesh::AddCircle(float a_fRadius, int a_nSubdivisions, vector3 offset, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	float angle = 0;
	// the change in the angle each time we draw a triangle
	float angleDelta = (2 * PI) / a_nSubdivisions;
	// when drawing a triangle, this is the point they share with the last triangle
	// the default value is for the first triangle only
	Simplex::vector3 anchor = Simplex::vector3(-a_fRadius, 0, 0) + offset;
	for (int i = 0; i <= a_nSubdivisions; i++, angle += angleDelta)
	{
		AddTri(offset, Simplex::vector3( -cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset, anchor);
		anchor = Simplex::vector3( -cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset;
	}
	// render the other side
	for (int i = 0; i <= a_nSubdivisions; i++, angle += angleDelta)
	{
		AddTri(Simplex::vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset, offset, anchor);
		anchor = Simplex::vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset;
	}
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

																									   // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
		//A--B
//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);

	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vTopRight);

}
void MyMesh::AddQuadRing(vector3 a_v3Offset, float a_fHeight, int a_nSubdivisions, float a_fRadius)
{
	float angle = 0;
	float angleDelta = (2 * PI) / a_nSubdivisions;
	for (int i = 0; i < a_nSubdivisions; ++i)
	{
		Simplex::vector3 p1 = Simplex::vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + a_v3Offset;
		Simplex::vector3 p2 = Simplex::vector3(-cos(angle + angleDelta) * a_fRadius, 0, -sin(angle + angleDelta) * a_fRadius) + a_v3Offset;
		Simplex::vector3 p3 = Simplex::vector3(-cos(angle) * a_fRadius, a_fHeight, -sin(angle) * a_fRadius) + a_v3Offset;
		Simplex::vector3 p4 = Simplex::vector3(-cos(angle + angleDelta) * a_fRadius, a_fHeight, -sin(angle + angleDelta) * a_fRadius) + a_v3Offset;
		AddQuad(p1, p2, p3, p4);
		angle += angleDelta;
	}

}
void MyMesh::AddQuadRing(vector3 a_v3Offset, float a_fHeight, int a_nSubdivisions, float a_fTopRadius, float a_fBottomRadius)
{
	float angle = 0;
	float angleDelta = (2 * PI) / a_nSubdivisions;
	for (int i = 0; i < a_nSubdivisions; ++i)
	{
		Simplex::vector3 p1 = Simplex::vector3(-cos(angle) * a_fBottomRadius, 0, -sin(angle) * a_fBottomRadius) + a_v3Offset;
		Simplex::vector3 p2 = Simplex::vector3(-cos(angle + angleDelta) * a_fBottomRadius, 0, -sin(angle + angleDelta) * a_fBottomRadius) + a_v3Offset;
		Simplex::vector3 p3 = Simplex::vector3(-cos(angle) * a_fTopRadius, a_fHeight, -sin(angle) * a_fTopRadius) + a_v3Offset;
		Simplex::vector3 p4 = Simplex::vector3(-cos(angle + angleDelta) * a_fTopRadius, a_fHeight, -sin(angle + angleDelta) * a_fTopRadius) + a_v3Offset;
		AddQuad(p1, p2, p3, p4);
		angle += angleDelta;
	}
}
void MyMesh::AddTubeEnd(float a_fInnerRadius, float a_fOuterRadius, float a_fHeight, int a_nSubdivisions)
{
	float angle = 0;
	float angleDelta = (2 * PI) / a_nSubdivisions;
	float deltaRadius = a_fOuterRadius - a_fInnerRadius;
	for (int i = 0; i < a_nSubdivisions; ++i, angle += angleDelta)
	{
		Simplex::vector3 p1 = Simplex::vector3(-cos(angle) * a_fInnerRadius, a_fHeight, -sin(angle) * a_fInnerRadius);
		Simplex::vector3 p2 = Simplex::vector3(-cos(angle + angleDelta) * a_fInnerRadius, a_fHeight, -sin(angle + angleDelta) * a_fInnerRadius);
		Simplex::vector3 p3 = Simplex::vector3(-cos(angle) * a_fOuterRadius, a_fHeight, -sin(angle) * a_fOuterRadius);
		Simplex::vector3 p4 = Simplex::vector3(-cos(angle + angleDelta) * a_fOuterRadius, a_fHeight, -sin(angle + angleDelta) * a_fOuterRadius);
		AddQuad(p1, p2, p3, p4);
	}
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color, bool compileGL)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;
	if (compileGL)
	{
		Release();
		Init();
	}
	
	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color, bool compileGL)
{
	if (compileGL)
	{
		Release();
		Init();
	}

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

													   //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}

void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 offset, vector3 a_v3Color, bool compileGL)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		//a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	if (compileGL)
	{
		Release();
		Init();
	}
	float angle = 0;
	// the change in the angle each time we draw a triangle
	float angleDelta = (2 * PI) / a_nSubdivisions;
	Simplex::vector3 anchor = Simplex::vector3(0, a_fHeight, 0) + offset;
	for (int i = 0; i <= a_nSubdivisions; i++, angle += angleDelta)
	{
		AddTri(vector3(Simplex::vector3(0, a_fHeight, 0) + offset), vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset, anchor);
		// have the triangle render on both sides
		AddTri(vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset, vector3(Simplex::vector3(0, a_fHeight, 0) + offset), anchor);
		anchor = Simplex::vector3(-cos(angle) * a_fRadius, 0, -sin(angle) * a_fRadius) + offset;
	}
	AddCircle(a_fRadius, a_nSubdivisions, offset, a_v3Color);
	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Offset, vector3 a_v3Color, bool compileGL)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	if (compileGL)
	{
		Release();
		Init();
	}
	AddCircle(a_fRadius, a_nSubdivisions, vector3(0, 0, 0) + a_v3Offset, a_v3Color);
	AddCircle(a_fRadius, a_nSubdivisions, vector3(0, a_fHeight, 0) + a_v3Offset, a_v3Color);
	// render the quads
	AddQuadRing(a_v3Offset, a_fHeight, a_nSubdivisions, a_fRadius);
	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}
void MyMesh::GenerateCylinder(float a_fTopRadius, float a_fBottomRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Offset, vector3 a_v3Color, bool compileGL)
{
	if (a_fTopRadius < 0.01f)
		a_fTopRadius = 0.01f;

	if (a_fBottomRadius < 0.005f)
		a_fBottomRadius = 0.005f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	if (compileGL)
	{
		Release();
		Init();
	}

	AddCircle(a_fTopRadius, a_nSubdivisions, vector3(0, a_fHeight, 0) + a_v3Offset, a_v3Color);
	AddCircle(a_fBottomRadius, a_nSubdivisions, vector3(0, 0, 0) + a_v3Offset, a_v3Color);
	AddQuadRing(a_v3Offset, a_fHeight, a_nSubdivisions, a_fTopRadius, a_fBottomRadius);
	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color, bool compileGL)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	if (compileGL)
	{
		Release();
		Init();
	}
	// add the inner and outer rings of quads
	AddQuadRing(vector3(0, 0, 0), a_fHeight, a_nSubdivisions, a_fInnerRadius);
	AddQuadRing(vector3(0, 0, 0), a_fHeight, a_nSubdivisions, a_fOuterRadius);
	// close the shape with the tube ends
	AddTubeEnd(a_fInnerRadius, a_fOuterRadius, a_fHeight, a_nSubdivisions);
	AddTubeEnd(a_fInnerRadius, a_fOuterRadius, 0, a_nSubdivisions);
	if (compileGL)
	{
		// Adding information about color
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	Mesh* pMesh = new Mesh();
	pMesh->GenerateTorus(a_fOuterRadius, a_fInnerRadius, a_nSubdivisionsA, a_nSubdivisionsB, a_v3Color);
	m_lVertexPos = pMesh->GetVertexList();
	m_uVertexCount = m_lVertexPos.size();
	SafeDelete(pMesh);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color, bool compileGL)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		//a_nSubdivisions = 6;

	if (compileGL)
	{
		Release();
		Init();
	}
	
	//Mesh* pMesh = new Mesh();
	//pMesh->GenerateSphere(a_fRadius, a_nSubdivisions, a_v3Color);
	//m_lVertexPos = pMesh->GetVertexList();
	//m_uVertexCount = m_lVertexPos.size();
	//SafeDelete(pMesh);
	// I will generate the sphere geometry based on circles stacked upon one another,
	// with them having smaller radii at the ends of the sphere and bigger radii toward
	// the equator. float* radii will store the radii of the imaginary circles,
	// heightDelta will be the difference in height between consecutive circles
	float* radii = new float[a_nSubdivisions];
	float heightDelta = ((a_fRadius) / (a_nSubdivisions));
	for (size_t i = 0; i < a_nSubdivisions; ++i)
	{
		radii[i] = sqrt(pow(a_fRadius, 2) - pow(i * heightDelta, 2));
		float test = radii[i];
	}
	int i;
	for (i = 0; i < a_nSubdivisions - 1; ++i)
	{
		AddQuadRing(vector3(0, i * heightDelta, 0), heightDelta, a_nSubdivisions, radii[i + 1], radii[i]);
	}
	for (i = 0; i < a_nSubdivisions - 1; ++i)
	{
		AddQuadRing(vector3(0, -i * heightDelta - heightDelta, 0), heightDelta, a_nSubdivisions, radii[i], radii[i + 1]);
	}
	// add the top and bottom cones
	GenerateCone(radii[i], heightDelta, a_nSubdivisions, vector3(0, a_fRadius - heightDelta, 0), vector3(1, 1, 1), false);
	GenerateCone(radii[i], -heightDelta, a_nSubdivisions, vector3(0, -(a_fRadius - heightDelta), 0), a_v3Color, false);
	// Adding information about color
	if (compileGL)
	{
		CompleteMesh(a_v3Color);
		CompileOpenGL3X();
	}
	delete radii;
}