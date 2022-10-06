#include "Client.h"
#include <Network/Minecraft/Common/Console.h>
#include <fstream>
#include <regex>
#include <future>
#include <chrono>


namespace stl
{
	//template<typename ValueType, typename  Container<ValueType>>
	//auto find(const typename Container<ValueType>& c, auto fn);


}

constexpr std::string_view DEFAULT_PORT = "27015";
struct Client
{
	Client()
	{

	}
	~Client()
	{

	}
	bool Connect(std::string_view address, std::string_view port = DEFAULT_PORT)
	{
		return m_Socket.Connect(address, port);
	}
	void Disconnect()
	{
		if (m_Socket.Disconnect())
		{
			m_thread.join();
		}
	}
	void Update()
	{
		m_Socket.Update();
	}
	void Send(Writer& stm)
	{
		Send(&stm.data[0], stm.len);
	}
	void Send(char* data, size_t len)
	{
		m_Socket.Send(data, len);
	}
	void Start()
	{
		m_Socket.OnData = std::move(OnData);
		m_Socket.OnDisconnect = std::move(OnDisconnect);
		m_thread = std::thread([this] {m_Socket.ThreadFunc(); });
	}

	std::string mail = "test@mail.com";
	std::string password = "123456";

	network::Socket m_Socket;
	network::System m_NetworkSystem;

	size_t id;
	std::thread m_thread;

	network::Socket::OnDataCb OnData;
	network::Socket::OnDisconnectCB OnDisconnect;

};

struct Friend
{
	size_t id;
	std::string name;
};

struct Message
{
	std::string from;
	std::string text;
};

namespace utils
{
	static int MyResizeCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			auto my_str = (std::string*)data->UserData;
			IM_ASSERT(my_str->c_str() == data->Buf);
			my_str->resize(data->BufSize/* - 1*/); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
			data->Buf = my_str->data();
		}
		return 0;
	}
	// Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
	// For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
	static bool MyInputTextMultiline(const char* label, std::string& my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		return ImGui::InputTextMultiline(label, my_str.data(), (size_t)my_str.size(), size, flags | ImGuiInputTextFlags_CallbackResize, utils::MyResizeCallback, (void*)&my_str);
	}

	static bool InputText(const char* label, std::string& my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		return ImGui::InputText(label, my_str.data(), (size_t)my_str.size(), flags | ImGuiInputTextFlags_CallbackResize, utils::MyResizeCallback, (void*)&my_str);
	}
}

struct Accounts
{
	void LoadUsers()
	{
		std::ifstream users("users.txt");
		if (users.is_open())
		{
			std::string line;
			static const std::regex re(R"((\w+),([a-zA-Z1-9@.]+),(.*),(.*),(.*))");
			while (std::getline(users, line))
			{
				std::smatch m;
				if (std::regex_search(line, m, re))
				{
					auto name = m[1];
					auto mail = m[2];
					auto pass = m[3];
					auto address = m[4];
					auto port = m[5];
					m_Users.push_back({ name,mail,pass,address,port });
				}
			}
		}
	}

	struct Info
	{
		User user;
		std::string address = "127.0.0.1";
		std::string port = "27015";
	};

	std::vector<Info> m_Users;
};

struct ConnectDialog
{
	std::function<bool(ConnectDialog&)> OnConnect;
	std::function<bool(ConnectDialog&)> OnRegister;
	ConnectDialog()
	{
		address = "127.0.0.1";
		port = "27015";

		accounts.LoadUsers();
	}
	void SelectAccount(int i)
	{
		auto& u = accounts.m_Users[i].user;
		mail = u.mail;
		password = u.password;
	}
	void AccountSettings()
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = true;
		static int dockspace_flags = 0;
		static bool p_open = true;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::BeginMenu("Accounts"))
				{
					for (int i = 0; i < accounts.m_Users.size(); i++)
					{
						if (ImGui::MenuItem(accounts.m_Users[i].user.mail.c_str()))
						{
							SelectAccount(i);
						}
					}
					ImGui::EndMenu();
				}
				//ImGui::MenuItem("Padding", NULL, &opt_padding);
				//ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

	}
	void Update()
	{

		if (isAutoConnect)
		{
			auto i = std::rand() % accounts.m_Users.size();
			SelectAccount(i);

			OnConnect(*this);
		}
		else
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
			bool p_open = true;
			ImGui::Begin("Connect to Server", &p_open, window_flags);
			{
				AccountSettings();
				utils::InputText("Address", address);
				utils::InputText("Port", port);
				utils::InputText("Mail", mail);
				utils::InputText("Password", password);

				if (ImGui::Button("Connect"))
				{
					OnConnect(*this);
				}
				ImGui::SameLine();
				if (ImGui::Button("Register"))
				{
					OnRegister(*this);
				}
				ImGui::End();
			}

		}


	}

	std::string address;
	std::string port;

	std::string mail;
	std::string password;

	Accounts accounts;

	bool isAutoConnect = 0000;
};

struct Chat
{
	//std::future<
	enum State
	{
		NotConnected,
		Connecting,
		Connected,
	};

	std::future<void> m_ConnectingFuture;
	Chat()
	{
		//m_online.push_back(Friend{ "HackMan", 1 });
		m_connectDialog.OnConnect = [this](ConnectDialog& cd) {
			auto onConnect = [this, &cd] { this->OnConnect(&cd); };

			m_ConnectingFuture = std::async(std::launch::async, onConnect);

			return true;
		};
		m_connectDialog.OnRegister = [this](ConnectDialog& cd) { return OnRegister(&cd); };
		m_console.OnInput = [this](ExampleAppConsole& console, const char* text) {
			//m_messages.push_back({ "You", text });
			console.AddLog("You: %s", text);
			SendMessageTo(text);
		};
	}
	~Chat()
	{
		m_client.Disconnect();
	}
	void DrawChat()
	{

	}
	void ConnectWindow()
	{
		m_connectDialog.Update();
	}
	void DrawMessages()
	{
		for (auto m : m_messages)
		{
			ImGui::TextWrapped("%s: %s", m.from.c_str(), m.text.c_str());
		}
	}
	void DrawFriends()
	{
		ImGui::Begin("Friends");
		for (auto& f : m_online)
		{
			if (ImGui::Button(f.name.c_str()))
			{

			}
		}
		ImGui::End();
	}
	void ChatWindow()
	{
		//UpdateClient();
		//utils::MyInputTextMultiline("##MyStr", text, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
		static bool p_open = true;
		m_console.Draw("Chat", &p_open);
		DrawFriends();

	}
	void UpdateClient()
	{
		m_client.Update();
	}
	void OnConnecting()
	{
		ImGui::Begin("Connect to Server", nullptr);
		{

			//const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
			//const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
			//const int spinner_radius = 40;

			//auto size = ImGui::GetWindowContentRegionMax();

			//auto hr = spinner_radius / 2;
			//ImGui::SetCursorPos(ImVec2(size.x / 2 - hr, size.y / 2 - hr));
			//ImGui::Spinner("##spinner", 40, 6, col);
			//ImGui::End();
		}
		//ImGui::End();
	}

	void Update()
	{
		UpdateClient();
		switch (m_state)
		{
		case Chat::NotConnected:
			ConnectWindow();
			break;
		case Chat::Connecting:
			ConnectWindow();
			OnConnecting();
			break;
		case Chat::Connected:
			ChatWindow();
			break;
		default:
			break;
		}
	}
	void SendMessageTo(std::string_view msg)
	{
		auto message = network::Client::MESSAGE;
		std::string_view data = msg;
		auto buffer_size = sizeof(message) + data.length() + 1 + sizeof(size_t);

		Writer wr(buffer_size);
		wr.Write(message);
		wr.Write(data);

		m_client.Send(wr);

	}
	void Auth(std::string_view mail, std::string_view password)
	{
#if 1
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#endif
#if 1
		auto message = network::Client::AUTH;
		auto buffer_size = sizeof(message) + mail.length() + password.length() + 2 * sizeof(size_t) + 2;

		Writer wr(buffer_size);
		wr.Write(message);
		wr.Write(mail);
		wr.Write(password);

		m_client.Send(wr);
#endif

	}
	void OnConnect(Stream& stm)
	{
		Friend f;
		stm.Read(f.id);
		stm.Read(f.name);

		m_online.push_back(f);
		m_console.AddLog("%s connected", f.name.c_str());
	}
	void OnDisconnect(Stream& stm)
	{
		size_t id;
		stm.Read(id);

		if (auto it = std::find_if(m_online.begin(), m_online.end(),
			[id](const Friend& f) {
			return f.id == id;
		}); it != m_online.end())
		{
			m_console.AddLog("%s disconnected", it->name.c_str());
			m_online.erase(it);
		}
	}
	void OnMessage(Stream& stm)
	{
		size_t id;
		std::string_view text;
		stm.Read(id);
		stm.Read(text);

		if (auto it = std::find_if(m_online.begin(), m_online.end(), [id](const Friend& f) {
			return f.id == id;
		}); it != m_online.end())
		{
			m_console.AddLog("%s: %s", it->name.c_str(), text.data());
		}
	}
	void ProcessIncomming(Stream& stm)
	{
#define HANDLE_MESSAGE(msg, fn) case network::Server::msg: (fn(stm)); break;
		auto EmtpyHandle = [](Stream& stm) {assert(0); };

		network::Server message;
		if (!stm.Read(message))
			return;
		switch (message)
		{
			HANDLE_MESSAGE(MESSAGE, OnMessage);
			HANDLE_MESSAGE(NAME, EmtpyHandle);
			HANDLE_MESSAGE(JOIN, EmtpyHandle);
			HANDLE_MESSAGE(CONNECT, OnConnect);
			HANDLE_MESSAGE(CONNECTED, [this](Stream& stm) {this->m_state = Chat::Connected; });
			HANDLE_MESSAGE(DISCONNECT, OnDisconnect);
		default:
			break;
		}

	}
	bool OnRegister(ConnectDialog* cd)
	{
		return false;
	}

	bool OnConnect(ConnectDialog* cd)
	{
		m_state = Connecting;
		if (auto result = m_client.Connect(cd->address, cd->port); result)
		{
			m_client.OnData = [this](char* data, size_t len) {
				Stream stm(data, len);
				ProcessIncomming(stm);
			};
			m_client.OnDisconnect = [this] {
				m_state = NotConnected;
				printf("Disconnection occur\n");
			};
			m_client.Start();

			Auth(cd->mail, cd->password);
			return true;
		}
		return false;
	}

	Client m_client;

	int m_currentFriend;
	std::vector<Friend> m_online;
	std::vector<Message> m_messages;
	State m_state = NotConnected;

	ConnectDialog m_connectDialog;
	// Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
// and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
	std::string text = "text";
	ExampleAppConsole m_console;
};

// Main code
const auto DEFAULT_WIDTH = 640;
const auto DEFAULT_HEIGHT = 480;
#if 0
int main(int, char**)
{
	Chat testChat;
	MainLoop mainLoop{ {"Client", DEFAULT_WIDTH, DEFAULT_HEIGHT}, [&testChat] {testChat.Update(); } };
	mainLoop.Start();
	return 0;
}
#endif
