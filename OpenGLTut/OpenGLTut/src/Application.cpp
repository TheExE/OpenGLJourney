#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Super duper cool window !", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Initialize GLEW once we have a valid openGL context */
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW !" << std::endl;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) <<
		std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
		};

		unsigned int indexes[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Bind vertex array object 
		VertexArray vertexArray;
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);

		// Position buffer
		const VertexBuffer vertexBuffer(positions,
			4/*vertex count*/ * 4/*floats per vertex*/ * sizeof(float));
		vertexArray.AddVertexBuffer(vertexBuffer, bufferLayout);
		
		// Position index buffer
		IndexBuffer indexBuffer(indexes, 6);

		Shader shader("res/shaders/Basic.shader");
		Renderer renderer;

		Texture texture("res/textures/OpenGLTextureTest.png");
		texture.Bind();
		shader.Bind();
		shader.SetUniform1i("u_texture", 0);

		vertexArray.Unbind();
		//shader.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			
			renderer.Clear();
			
			//shader.Bind();
			//shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);
			renderer.Draw(vertexArray, indexBuffer);

			r += increment;
			if (r > 1)
			{
				increment = -0.05f;
			}
			else if (r < 0)
			{
				increment = 0.05f;
			}


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();

	return 0;
}
