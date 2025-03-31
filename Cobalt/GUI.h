#pragma once
#include <../includes.h>
#include <iostream>
#include <curl.h>
#include <unzip.h>
#include <../json.hpp>
#include <../stb_image.h>
#include <filesystem>
#include <vector>
#include <thread>
#pragma warning(disable:2672)
namespace fs = std::filesystem;
#pragma warning(disable:4996)
using json = nlohmann::json;
namespace Cobalt
{
	class GUI
	{
	public:
		static DWORD WelcomeWindow()
		{
			ImGuiIO io = ImGui::GetIO();
			ImDrawList* draw = ImGui::GetBackgroundDrawList();
			draw->AddRectFilled(ImVec2(0,0), io.DisplaySize, IM_COL32(255, 0, 0, 255));
			float LaunchButton = ImGui::Button("Open Cobalt Button");
			ImGui::Text("Cobalt");
			ImGui::NextColumn();
			ImGui::Text("Welcome To Cobalt Made By Ryse404");
			ImGui::NextColumn();
			LaunchButton;
			return 0;
		}
		
		static DWORD ExitButton()
		{
			bool show_exit_button = true;
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 50, 10), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(50, 0), ImGuiCond_Always);
			ImGui::Begin("Exit Button", &show_exit_button, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
			if (ImGui::Button("X"))
			{
				exit(0);
			}
			ImGui::End();
			
		}

		void LaunchFortnite()
		{
			std::string fortnitegamepath = "FortniteGame\\Binaries\\Win64\\FortniteClient-Win64-Shipping.exe";
			if (ShellExecuteA(NULL, "open", fortnitegamepath.c_str(), NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
			{
				std::cerr << "ERROR: CANNOT LAUNCH FORTNITE";
			}
		}

		void RenderLaunchPage()
		{
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), IM_COL32(0, 0, 0, 255));
			ImGui::Text("Welcome To Cobalt");
			if (ImGui::Button("Launch Fortnite"))
			{
				LaunchFortnite();
			}
		}

		static DWORD LaunchFortnitePage()
		{
			while (true)
			{
				ImGui::NewFrame();
				&RenderLaunchPage;
				ImGui::Render();
			}

			return 0;
		}

		const std::string cobaltGamesFolder = getenv("USERPROFILE") + std::string("\\Desktop\\CobaltFortniteGames\\");
		const std::string versionsFilePath = getenv("LOCALAPPDATA") + std::string("\\VersionsDownloaded.json");
		const std::vector<std::string> fortniteUrls = {

		};

		size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
			((std::string*)userp)->append((char*)contents, size * nmemb);
			return size * nmemb;
		}

		const static bool DownloadFile(const std::string& url, const std::string& outputPath)
		{
			CURL* curl;
			FILE* fp;
			CURLcode res;
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();

			if (curl) {
				fp = fopen(outputPath.c_str(), "wb");
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

				res = curl_easy_perform(curl);
				if (res != CURLE_OK) {
					std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
					fclose(fp);
					curl_easy_cleanup(curl);
					curl_global_cleanup();
					return false;
				}

				fclose(fp);
				curl_easy_cleanup(curl);
				curl_global_cleanup();
				return true;
			}
			return false;
		}

		bool ExtractZip(const std::string& zipFilePath, const std::string& outputDir)
		{
			unzFile zipFile = unzOpen(zipFilePath.c_str());
			if (!zipFile)
			{
				std::cerr << "Failed to extract" << std::endl;
				return false;
			}

			if (unzGoToFirstFile(zipFile) != UNZ_OK)
			{
				std::cerr << "No filed found" << std::endl;
				unzClose(zipFile);
				return false;
			}

			do
			{
				char fileName[256];
				unz_file_info fileInfo;
				if (unzGetCurrentFileInfo(zipFile, &fileInfo, fileName, sizeof(fileName), NULL, 0, NULL, 0) != UNZ_OK)
				{
					std::cerr << "Failed to get file info" << std::endl;
					continue;
				}

				std::string outputFilePath = outputDir + fileName;
				FILE* outFile = (fopen(outputFilePath.c_str(), "wb"));
				if (!outFile)
				{
					std::cerr << "Failed to open file extraction" << std::endl;
					continue;
				}
				if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
					std::cerr << "Failed to open file in zip!" << std::endl;
					fclose(outFile);
					continue;
				}

				char buffer[4096];
				int bytesRead;
				while ((bytesRead = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0)
				{
					fwrite(buffer, 1, bytesRead, outFile);
				}
				fclose(outFile);
				unzCloseCurrentFile(zipFile);
			} while (unzGoToNextFile(zipFile) == UNZ_OK);

			unzClose(zipFile);
			return true;
		}

		void SaveDownloadInfo(const std::string& versionName, const std::string& filePath)
		{
			json downloadData;
			// Load existing data if available
			std::ifstream inFile(versionsFilePath);
			if (inFile.is_open()) {
				inFile >> downloadData;
				inFile.close();
			}

			// Add new version info
			downloadData["versions"].push_back({
				{"version_name", versionName},
				{"file_path", filePath}
				});

			// Save the updated data
			std::ofstream outFile(versionsFilePath);
			if (outFile.is_open()) {
				outFile << downloadData.dump(4);
				outFile.close();
			}
		}
		void RenderDownloadPage() {
			// Set background to black
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), IM_COL32(0, 0, 0, 255));

			ImGui::Text("Download Fortnite Versions");

			for (size_t i = 0; i < fortniteUrls.size(); ++i) {
				std::string versionName = "Version " + std::to_string(i + 1);
				if (ImGui::Button(("Download " + versionName).c_str())) {
					std::string outputZipPath = cobaltGamesFolder + versionName + ".zip";

					// Download the version
					if (DownloadFile(fortniteUrls[i], outputZipPath)) {
						std::cout << "Downloaded: " << versionName << std::endl;

						// Extract the downloaded file
						if (ExtractZip(outputZipPath, cobaltGamesFolder + versionName + "\\")) {
							std::cout << "Extracted: " << versionName << std::endl;
							SaveDownloadInfo(versionName, cobaltGamesFolder + versionName);
						}
					}
					else {
						std::cerr << "Failed to download: " << versionName << std::endl;
					}
				}
			}
		}

		static DWORD DownloadFortnitePage()
		{
			while (true)
			{
				ImGui::NewFrame();
				&RenderDownloadPage;
				ImGui::Render();
			}

			return 0;
		}

		std::vector<std::string> fortniteVersions;
		int selectedVersionIndex = 0;
		void LoadVersionImages()
		{
			std::string splashDir = "FortniteGame\\Content\\Splash\\Splash.bmp";
			for (const auto& entry : fs::directory_iterator(splashDir))
			{
				if (entry.is_regular_file())
				{
					std::string extension = entry.path().extension().string();
					if (extension == "bmp")
					{
						fortniteVersions.push_back(entry.path().string());
					}
				}
			}
		}
		GLuint LoadTextureFromFile(const char* filename)
		{
			int width, height, channels;
			unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
			if (!data) {
				std::cerr << "Failed to load image: " << filename << std::endl;
				return 0;
			}
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
			return textureID;
		}
		void RenderFortniteGameLibraryPage() {
			// Set background to black
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), IM_COL32(0, 0, 0, 255));

			// Display the selected version image
			if (!fortniteVersions.empty() && selectedVersionIndex < fortniteVersions.size()) {
				std::string imagePath = fortniteVersions[selectedVersionIndex];
				GLuint textureID = LoadTextureFromFile(imagePath.c_str());

				ImVec2 imageSize(300, 300);
				ImGui::Image((void*)(intptr_t)textureID, imageSize);

				// Display version text below the image
				std::string versionText = "FNVersion" + std::to_string(selectedVersionIndex + 1);
				ImGui::Text(versionText.c_str());

				// Slider to select different versions
				if (fortniteVersions.size() > 5) {
					ImGui::SameLine(ImGui::GetWindowWidth() - 300);  // Place slider on the right side
					//ImGui::PushStyleVar(ImGuiStyleVar.FrameRounding, 10.0f); // Border radius of the slider
					ImGui::SliderInt("##VersionSlider", &selectedVersionIndex, 0, fortniteVersions.size() - 1);
					ImGui::PopStyleVar();
				}
			}
		}
		static DWORD FortniteGameLibrary()
		{
			&LoadVersionImages;
			while (true)
			{
				ImGui::NewFrame();
				&RenderFortniteGameLibraryPage;
				ImGui::Render();
			}
		}

		static DWORD SettingsPage()
		{

		}

		struct Language
		{
			std::string name;
			std::string flagImagePath;
			GLuint flagTextureID;
		};

		std::vector<Language> languages;

		GLuint LoadTextureFromFile2(const char* filename)
		{
			int width, height, channels;
			unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
			if (!data) {
				std::cerr << "Failed to load image: " << filename << std::endl;
				return 0;
			}

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
			return textureID;
		}

		void LoadLanguageFlags() {
			// Add languages and their corresponding flag images (example paths)
			languages.push_back({ "English", "path_to_flags/english_flag.png", 0 });
			languages.push_back({ "Spanish", "path_to_flags/spanish_flag.png", 0 });
			languages.push_back({ "French", "path_to_flags/french_flag.png", 0 });
			languages.push_back({ "German", "path_to_flags/german_flag.png", 0 });
			languages.push_back({ "Italian", "path_to_flags/italian_flag.png", 0 });
			languages.push_back({ "Portuguese", "path_to_flags/portuguese_flag.png", 0 });

			// Load textures for each flag
			for (auto& lang : languages) {
				lang.flagTextureID = LoadTextureFromFile(lang.flagImagePath.c_str());
			}
		}

		void RenderLanguagePage()
		{
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), IM_COL32(0, 0, 0, 255));

			// ComboBox for language selection
			const char* comboBoxLabel = "Select Language";
			static int selectedIndex = 0; // Store the selected language index

			// Prepare the language names for the combo box
			std::vector<const char*> languageNames;
			for (const auto& lang : languages) {
				languageNames.push_back(lang.name.c_str());
			}

			// ComboBox to select the language
			if (ImGui::BeginCombo(comboBoxLabel, languages[selectedIndex].name.c_str())) {
				for (int i = 0; i < languages.size(); i++) {
					bool isSelected = (selectedIndex == i);
					if (ImGui::Selectable(languages[i].name.c_str(), isSelected)) {
						selectedIndex = i;
					}

					// Display the flag next to the language name
					if (isSelected) {
						ImVec2 flagSize(30, 30);
						ImGui::SameLine();
						ImGui::Image((void*)(intptr_t)languages[i].flagTextureID, flagSize);
					}
				}
				ImGui::EndCombo();
			}

			// Display the selected language and its flag in a separate line (Optional)
			ImGui::Text("Selected Language: %s", languages[selectedIndex].name.c_str());
			ImVec2 flagSize(30, 30);
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)languages[selectedIndex].flagTextureID, flagSize);
		}
		static DWORD LanguagePage()
		{
			&LoadLanguageFlags;

			while (true)
			{
				ImGui::NewFrame();
				&RenderLanguagePage;
				ImGui::Render();
			}

			return 0;
		}
		std::string updateURL = "https://cobalt.xyz/updates";
		void DeleteInternetShortcut() {
			bool isUpdateInProgress = false;
			float downloadProgress = 0.0f;
			float downloadSpeed = 0.0f;
			bool updateDone = false;
			bool relaunchApp = false;
			std::string shortcutPath = "C:\\Users\\" + std::string(getenv("USERNAME")) + "\\Desktop\\Cobalt.lnk";
			if (DeleteFileA(shortcutPath.c_str())) {
				std::cout << "Deleted shortcut successfully!" << std::endl;
			}
		}
		void RestoreInternetShortcut() {
			bool isUpdateInProgress = false;
			float downloadProgress = 0.0f;
			float downloadSpeed = 0.0f;
			bool updateDone = false;
			bool relaunchApp = false;
			std::string shortcutPath = "C:\\Users\\" + std::string(getenv("USERNAME")) + "\\Desktop\\Cobalt.lnk";
			std::ofstream outfile(shortcutPath);
			if (outfile.is_open()) {
				outfile << "[InternetShortcut]\nURL=" << updateURL << "\n";
				outfile.close();
				std::cout << "Restored shortcut successfully!" << std::endl;
			}
		}
		static int WriteCallback2(void* contents, size_t size, size_t nmemb, void* userp) {
			size_t totalSize = size * nmemb;
			bool isUpdateInProgress = false;
			float downloadProgress = 0.0f;
			float downloadSpeed = 0.0f;
			bool updateDone = false;
			bool relaunchApp = false;
			downloadProgress += (float)totalSize / 1000000.0f; // Simulated download progress
			return totalSize;
		}
		size_t WriteData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
			size_t written = fwrite(ptr, size, nmemb, stream);
			return written;
		}
		void DownloadUpdate() {
			CURL* curl;
			FILE* fp;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();

			if (curl) {
				fp = fopen("update.zip", "wb");
				curl_easy_setopt(curl, CURLOPT_URL, updateURL.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
				curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION);
				curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, nullptr);

				res = curl_easy_perform(curl);

				if (res != CURLE_OK) {
					std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
				}
				else {
					std::cout << "Download succeeded!" << std::endl;
				}

				fclose(fp);
				curl_easy_cleanup(curl);
			}
			curl_global_cleanup();
		}
		void RenderUpdatePage() {
			bool isUpdateInProgress = false;
			float downloadProgress = 0.0f;
			float downloadSpeed = 0.0f;
			bool updateDone = false;
			bool relaunchApp = false;
			std::string updateURL = "https://cobalt.xyz";
			// Create window for updates
			ImGui::SetNextWindowSize(ImVec2(700, 700), ImGuiCond_Always);
			ImGui::Begin("Cobalt Update Out!");

			// Title text
			ImGui::Text("Cobalt Update Out!");
			ImGui::Separator();

			// Show download progress and speed
			if (isUpdateInProgress) {
				ImGui::Text("Downloading Update...");
				ImGui::ProgressBar(downloadProgress, ImVec2(0.0f, 0.0f), std::to_string(int(downloadProgress * 100)).c_str());

				ImGui::Text("Download Speed: %.2f KB/s", downloadSpeed);  // Simulating speed calculation
			}
			else {
				ImGui::Text("No Update in Progress");
			}

			// Update Button (starts update process)
			if (ImGui::Button("Start Update") && !isUpdateInProgress) {
				isUpdateInProgress = true;
				downloadProgress = 0.0f;
				downloadSpeed = 50.0f;  // Simulated download speed (KB/s)
				DeleteInternetShortcut();

				// Download in a separate thread to avoid blocking the UI
			}

			// Show update done message after download
			if (updateDone) {
				ImGui::Text("Update Done!");
				if (ImGui::Button("Close and Relaunch")) {
					RestoreInternetShortcut();
					relaunchApp = true;
				}
			}

			ImGui::End();
		}
		void UpdateLoop() {
			bool isUpdateInProgress = false;
			float downloadProgress = 0.0f;
			float downloadSpeed = 0.0f;
			bool updateDone = false;
			bool relaunchApp = false;
			std::string updateURL = "https://cobalt.xyz";
			if (downloadProgress >= 1.0f) {
				updateDone = true;
				// Simulate application closing and relaunch
				std::this_thread::sleep_for(std::chrono::seconds(4));

				if (relaunchApp) {
					// Simulate application relaunch
					// Process to restart the application
					ShellExecuteA(NULL, "open", "Cobalt.exe", NULL, NULL, SW_SHOWNORMAL);
					PostQuitMessage(0);  // Exit the application
				}
			}
		}
		static DWORD UpdatePage()
		{
			while (true)
			{
				ImGui::NewFrame();
				&RenderUpdatePage;
				ImGui::Render();
				&UpdateLoop;
			}

			return 0;
		}
#define INI_FILE_PATH "%localappdata%\\CobaltUser\\CobaltInformation.ini"
		void OpenURL(const std::string& url) {
			ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		bool ReadUserData(std::string& email, std::string& password) {
			std::ifstream infile(INI_FILE_PATH);
			if (!infile.is_open()) {
				return false; // File doesn't exist
			}

			std::string line;
			while (std::getline(infile, line)) {
				if (line.find("[AUTH]") != std::string::npos) {
					// Read the email and password from the [AUTH] section
					std::getline(infile, line);  // Email
					email = line.substr(line.find('=') + 1); // Email value after '='
					std::getline(infile, line);  // Password
					password = line.substr(line.find('=') + 1); // Password value after '='
					return true;
				}
			}
			return false; // No [AUTH] section found
		}
		void SaveUserData(const std::string& email, const std::string& password) {
			// Ensure the directory exists
			std::filesystem::create_directory("C:\\Users\\" + std::string(getenv("USERNAME")) + "\\AppData\\Local\\CobaltUser");

			// Open the file for writing
			std::ofstream outfile(INI_FILE_PATH);
			if (outfile.is_open()) {
				outfile << "[AUTH]\n";
				outfile << "email=" << email << "\n";
				outfile << "password=" << password << "\n";
			}
		}
		void RenderUserLoginPage() {
			static char email[128] = "";
			static char password[128] = "";
			static bool loginSuccessful = false;

			// Read saved user data from the file
			if (!loginSuccessful) {
				std::string savedEmail, savedPassword;
				if (ReadUserData(savedEmail, savedPassword)) {
					strcpy_s(email, savedEmail.c_str());
					strcpy_s(password, savedPassword.c_str());
					loginSuccessful = true;
				}
			}

			ImGui::Text("Login to Google Account");

			// Email Input
			ImGui::InputText("Email", email, IM_ARRAYSIZE(email));

			// Password Input (password box)
			ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

			// "Login" Button
			if (ImGui::Button("Login")) {
				// Here, you could implement validation with Google APIs if you wish
				// For now, we're assuming login is successful if the user submits the form

				// Save email and password to the configuration file
				SaveUserData(email, password);
				ImGui::Text("Login successful! Information saved.");
			}

			// Open Google login page URL
			if (ImGui::Button("Open Google Login")) {
				OpenURL("https://accounts.google.com/ServiceLogin");
			}
		}
		static DWORD UserLoginPage()
		{
			while (true)
			{
				ImGui::NewFrame();
				&RenderUserLoginPage;
				ImGui::Render();
			}

			return 0;
		}
	};
}