// DialDRM main.cpp, code might be very unoptimized

#include <imgui_vita.h>
#include <stdio.h>
#include <vitaGL.h>
#include <stdlib.h>
#include <vitasdk.h>
#include <psp2/ctrl.h>
#include <psp2/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	uint8_t *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

int main(int, char**)
{
	unsigned int _newlib_heap_size_user = 2 * 1024 * 1024;

	vglInitExtended(0x100000, 960, 544, 0x200000, SCE_GXM_MULTISAMPLE_4X);

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplVitaGL_Init();

	// Setup style
	ImGui::StyleColorsDark();

	ImVec4 clear_color = ImVec4(70.0f/255.0f, 69.0f/255.0f, 70.0f/255.0f, 1.00f);

	ImGui_ImplVitaGL_TouchUsage(true);
	ImGui_ImplVitaGL_UseIndirectFrontTouch(true);

	//Variables
	bool somi = false;
	float rotation = 0.0f;
	int my_image_widthA = 0;
	int my_image_heightA = 0;
	GLuint my_image_textureA = 0;

	int my_image_widthB = 0;
	int my_image_heightB = 0;
	GLuint my_image_textureB = 0;

	int my_image_widthC = 0;
	int my_image_heightC = 0;
	GLuint my_image_textureC = 0;

	glEnable(GL_TEXTURE_2D);

	// Main loop
	bool done = false;
	while (!done)
	{
		vglStartRendering();
		ImGui_ImplVitaGL_NewFrame();
		

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Games")) {
				if (ImGui::MenuItem("Secret of Monkey Island")) { somi = true; 
					bool ret1 = LoadTextureFromFile("app0:games/somi/somiA.png", &my_image_textureA, &my_image_widthA, &my_image_heightA);
					bool ret2 = LoadTextureFromFile("app0:games/somi/somiB.png", &my_image_textureB, &my_image_widthB, &my_image_heightB);
					bool ret3 = LoadTextureFromFile("app0:games/somi/somi.png", &my_image_textureC, &my_image_widthC, &my_image_heightC);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//Load info bar
		if (somi)
		{
			ImGui::SetNextWindowPos(ImVec2(my_image_widthA, 19), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(960 - my_image_widthA , 525), ImGuiSetCond_Always);
			ImGui::Begin("Selector Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::Image((void*)(intptr_t)my_image_textureC, ImVec2(my_image_widthC, my_image_heightC));
			ImGui::PushItemWidth(-1.0f);
			ImGui::Text("Dial Rotation:");
			ImGui::SliderFloat("##Rotation", &rotation, -180.0f, 180.0f, "%.0f", 1.0f);
			ImGui::Text( "Dial-A-Pirate");
			ImGui::TextWrapped("This form of copy-protection was included in early floppy disk releases of the game. It is a cardboard wheel, named ""\x0022""Dial-a-Pirate\x0022, was provided, and the player had to match the pirate shown on-screen with that of the wheel.\nInstrunctions from game manual:");
			ImGui::Spacing();
			ImGui::TextWrapped("Once you\x0027ve started a program. a screen will appear displaying pirate\x0027s face (actually a combination of two faces). You will be prompted to enter a date that was significant in the pirate\x0027s life at a given geographical location. Use your Dial-A-Pirate wheel to match up the top and bottom halves of the pirate face you see on the screen. Then, locate the window on the wheel that matches the geographical location mentiones on the screen. Using the keyboard, type the date you see in the window. Don\x0027t lose your Dial-A-Pirate wheel! Without it, you won\x0027t be able to play the game.If you lose the Wheel, you may purchase another from Lucasfilm Games.");
			
			ImGui::End();

		}
		
		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		if (somi)
		{
			//Create bottom disc
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 960, 544, 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glBindTexture(GL_TEXTURE_2D, my_image_textureA);

			glBegin(GL_QUADS);

			glTexCoord2i(0, 0);
			glVertex3f(0, 19, 0);
			glTexCoord2i(1, 0);
			glVertex3f(my_image_widthA , 19, 0);
			glTexCoord2i(1, 1);
			glVertex3f(my_image_widthA, my_image_heightA + 19, 0);
			glTexCoord2i(0, 1);
			glVertex3f(0, my_image_heightA + 19, 0);

			glEnd();

			//Create top disc
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 960, 544, 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glBindTexture(GL_TEXTURE_2D, my_image_textureB);

			//Rotate top disc

			if (rotation != 0)
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef((float)my_image_widthB / 2.0f, ((float)my_image_heightB / 2.0f)+ 19.0f, 0.0f);
				glRotatef((float)rotation, 0.0f, 0.0f, 1.0f);
				glTranslatef(-(float)my_image_widthB / 2.0f, -(((float)my_image_heightB / 2.0f)+19.0f), 0.0f);
			}

			glBegin(GL_QUADS);

			glTexCoord2i(0, 0);
			glVertex3f(0, 19, 0);
			glTexCoord2i(1, 0);
			glVertex3f(my_image_widthB, 19, 0);
			glTexCoord2i(1, 1);
			glVertex3f(my_image_widthB, my_image_heightB + 19, 0);
			glTexCoord2i(0, 1);
			glVertex3f(0, my_image_heightB + 19, 0);

			glEnd();
		}
		ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());

		vglStopRendering();
	}

	// Cleanup
	ImGui_ImplVitaGL_Shutdown();
	ImGui::DestroyContext();
	
	vglEnd();

	return 0;
}
