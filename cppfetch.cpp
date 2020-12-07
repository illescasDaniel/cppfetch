#include <cstddef>
#include <fstream>
#include <iostream>
#include <future>
#include <cstdlib>
#include <cstring>

#include <array>
#include <memory>
#include <string>

// system functions
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>

using namespace std;

inline int sync_system_call_str(std::string&& command);
inline int sync_system_call_str(std::string& command);
inline future<int> system_call(const char* command);
inline future<int> system_call_str(std::string& command);
inline future<int> system_call_str(std::string&& command);
inline future<int> color_print(std::string&& colored_text, std::string&& normal_text);
inline future<int> color_print2(std::string colored_text, std::string normal_text);

inline int print_newline();
inline int print_os_name();
inline int print_colors();
inline future<int> print_name();

void print_empty_line();
future<void> print_colors_2();
future<void> print_full_name();
future<void> print_os_name_2();
future<void> print_kernel_version();
future<void> print_desktop_env();

inline std::string get_colors();
inline std::string get_os_name();
inline std::string get_full_os_name();
inline std::string get_kernel_version();
inline std::string get_uptime();
inline std::string get_packages();
inline std::string get_pm_packages();
inline std::string get_shell();
inline std::string get_resolution();
inline std::string get_desktop_env();
inline std::string get_wm();
inline std::string get_theme();
inline std::string get_icons();
inline std::string get_cursor();
inline std::string get_font();
inline std::string get_terminal();
inline std::string get_terminal_font();
inline std::string get_cpu();
inline std::string get_gpu();
inline std::string get_memory();
inline std::string get_gpu_driver();

const std::string left_padding = "  ";

int main(int argc, char* argv[]) {

	print_os_name();
	print_empty_line();
	print_colors_2().wait();
	print_empty_line();
	print_full_name().wait();
	std::cout << left_padding << "----------------------" << endl;
	
	auto os_name = print_os_name_2();//color_print("OS", get_full_os_name());
	auto kernel_version = print_kernel_version();//color_print("Kernel", get_kernel_version());
	auto uptime = color_print2("Uptime", get_uptime());
	auto shell_name = color_print2("Shell", get_shell());
	auto packages = color_print("Packages", get_pm_packages());
	auto resolution = color_print("Resolution", get_resolution());
	auto desktop_env = print_desktop_env();//color_print("Desktop Environment", get_desktop_env());
	auto window_manager = color_print("Window Manager", get_wm());
	auto theme = color_print("Theme", get_theme());
	auto icons = color_print("Icons", get_icons());
	auto cursor = color_print("Cursor", get_cursor());
	auto font = color_print("Font", get_font());
	auto terminal = color_print("Terminal", get_terminal());
	auto terminal_font = color_print("Terminal Font", get_terminal_font());
	auto cpu = color_print("CPU", get_cpu());
	auto gpu = color_print("GPU", get_gpu());
	auto memory = color_print("Memory", get_memory());
	auto gpu_driver = color_print("Nvidia GPU Driver", get_gpu_driver());
	
	// os_name.get();
	// use get to get result, wait to wait to finish
}

//
const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string& s)
{
    return rtrim(ltrim(s));
}

// https://stackoverflow.com/a/478960/6303785
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


//

inline future<int> system_call(const char* command) {
	return std::async(std::launch::async, [&] {
		std::cout << exec(command) << std::flush;
		return 0;
	});
}

inline future<int> system_call_str(std::string& command) {

	int len = command.size();
	char *c = new char[len + 1];
	std::copy(command.begin(), command.end(), c);
	c[len] = '\0';

	return std::async(std::launch::async, system, c);
}

inline future<int> system_call_str(std::string&& command) {
	return system_call(command.c_str());
}

inline int sync_system_call_str(std::string&& command) {
	std::cout << exec(command.c_str()) << std::flush;
	return 0;
}

inline int sync_system_call_str(std::string& command) {
	std::cout << exec(command.c_str()) << std::flush;
	return 0;
}

inline future<int> color_print(std::string&& colored_text, std::string&& normal_text) {
	
	std::string green = "\033[0;32m";
	std::string normal = "\033[0m";

	std::string final_command = "echo \""+left_padding+green+colored_text+normal+": $("+normal_text+")\"";
	
	return system_call_str(final_command);
}

inline future<int> color_print2(std::string colored_text, std::string normal_text) {
	
	std::string green = "\033[0;32m";
	std::string normal = "\033[0m";

	return std::async(std::launch::async, [&] {
		const auto command_output = exec(normal_text.c_str());//strdup(normal_text.c_str());
		std::cout << left_padding << colored_text << ": " << trim(command_output) << std::endl << std::flush;
		return 0;
	});
}

inline void color_print3(std::string colored_text, std::string normal_text) {
	
	std::string green = "\033[0;32m";
	std::string normal = "\033[0m";

	std::cout << left_padding << colored_text << ": " << trim(normal_text) << std::endl << std::flush;
}

//

inline int print_os_name() {
	return sync_system_call_str(std::string("/usr/bin/figlet ")+std::string("$("+get_os_name()+")"));
}

inline future<int> print_name() {

	std::string name = "$(whoami)";
	std::string at = "@";
	std::string pc_name = "$(uname --nodename)";

	std::string green = "\033[0;32m";
	std::string normal = "\033[0m";
	std::string final_command = "echo \""+left_padding+green+name+normal+"@"+green+pc_name+normal+"\n"+left_padding+"----------------------------\"";
	
	return system_call_str(final_command);
}

inline std::string get_colors() {

	std::string sp = "   ";
	std::string b = "\033[0;40m", r = "\033[0;41m", g = "\033[0;42m", y = "\033[1;43m", blue = "\033[0;44m", m = "\033[0;45m", c = "\033[0;46m", w = "\033[1;47m", normal = "\033[0m";
	std::string brb = "\033[0;100m", brr = "\033[0;101m", brg = "\033[0;102m", bry = "\033[1;103m", brblue = "\033[0;104m", brm = "\033[0;105m", brc = "\033[0;106m", brw = "\033[1;107m";

	return b+sp + r+sp + g+sp + y+sp + blue+sp + m+sp + c+sp + w+sp + normal + "\n  " +
		   brb+sp + brr+sp + brg+sp + bry+sp + brblue+sp + brm+sp + brc+sp + brw+sp + normal;
}

inline int print_colors() {
	return sync_system_call_str("echo \"\n  "+get_colors()+"\n\"");
}

inline std::string get_os_name() {
	return "cat /etc/*-release | head -n 1";
}

//

std::string get_user_name()
{
	uid_t userid;
	struct passwd* pwd;
	userid = getuid();
	pwd = getpwuid(userid);
	return pwd->pw_name;
}

std::string get_kernel_version_2()
{
	struct utsname name;
	uname(&name);
	return name.release;
}

std::string get_user_node_name() {
	struct utsname name;
	uname(&name);
	return name.nodename;
}

inline std::string get_os_name2()
{

	ifstream stream = ifstream();

	stream.open("/etc/arch-release");
	if (!stream.fail()) {
		std::string os_name_str = "";
		std::getline(stream, os_name_str);
		os_name_str = trim(os_name_str);
		stream.close();
		if (!os_name_str.empty()) {
			return os_name_str;
		}
	}

	stream.open("/etc/lsb-release");
	if (!stream.fail()) {
		std::string line = "";
		std::string valid_os_name = "";
		bool found = false;

		while(std::getline(stream, line) && !found) {	
			if (line.find("DISTRIB_DESCRIPTION") != std::string::npos) {
				found = true;
				valid_os_name = line;
			}
		}
		
		if (found) {
			
			const size_t equal_pos = valid_os_name.find("=");
			if (equal_pos != std::string::npos) {
				
				std::string name = valid_os_name.substr(equal_pos+1);
			
				if (name[0] == '"' && name[name.length()-1] == '"') {
					name = name.substr(1, name.length()-2);
				}
				name = trim(name);
				stream.close();
				if (!name.empty()) {
					return name;
				}
			}
		}
	}

	stream.close();
	return "(unknown)";
}

inline future<void> print_os_name_2() 
{
	return std::async(std::launch::async, []{
		color_print3("OS", get_os_name2());
	});
}

inline future<void> print_kernel_version() 
{
	return std::async(std::launch::async, []{
		color_print3("Kernel", get_kernel_version_2());
	});
}

inline future<void> print_desktop_env() 
{
	return std::async(std::launch::async, []{
		color_print3("Desktop Environment", getenv("XDG_CURRENT_DESKTOP"));
	});
}

inline future<void> print_full_name() 
{
	return std::async(std::launch::async, []{
		// TODO: print it in color
		std::cout << left_padding << get_user_name() << '@' << get_user_node_name() << std::endl << std::flush;
	});	
}

inline future<void> print_colors_2() 
{
	return std::async(std::launch::async, []{
		std::cout << left_padding << get_colors() << std::endl << std::flush;
	});	
}

inline void print_empty_line()
{
	std::cout << std::endl << std::flush;
}







inline std::string get_full_os_name() {
	std::string os_name = "$(cat /etc/*-release | head -n 1)";
	std::string architecture = "$(uname --machine)";
	return "echo \""+os_name+" "+architecture+"\"";
}

inline std::string get_kernel_version() {
	return "uname --kernel-release";
}

inline std::string get_uptime() {
	return "uptime --pretty";
}

inline std::string get_packages() {
	std::string pacman_packages = "/usr/bin/pacman -Qq | wc -l";
	std::string flatpak_packages = "/usr/bin/flatpak list | wc -l";
	return "echo \"$("+pacman_packages+") (pacman), $("+flatpak_packages+")(flatpak)\"";
}

inline std::string get_pm_packages() {
	std::string pacman_packages = "/usr/bin/pacman -Qq | wc -l";
	return "echo \"$("+pacman_packages+") (pacman)\"";
}


inline std::string get_resolution() {
	std::string resolution = "xrandr | awk '/\\*/ {print $1 \" @ \" $2}'";
	// todo: clean resolution asterisc
	return resolution;
}

inline std::string get_shell() {
	return "fish --version";
}

inline std::string get_desktop_env() {
	return "printf \"$XDG_CURRENT_DESKTOP\"";
}

inline std::string get_wm() {
	return "wmctrl -m | grep Name | awk '{print $2}'";
}

inline std::string get_theme() {
	return "awk -F '=' '/gtk-theme-name/ {print $2}' ~/.config/gtk-3.0/settings.ini";
}

inline std::string get_icons() {
	return "awk -F '=' '/gtk-icon-theme-name/ {print $2}' ~/.config/gtk-3.0/settings.ini";
}

inline std::string get_cursor() {
	return "awk -F '=' '/gtk-cursor-theme-name/ {print $2}' ~/.config/gtk-3.0/settings.ini";
}

inline std::string get_font() {
	return "awk -F '=' '/gtk-font-name/ {print $2}' ~/.config/gtk-3.0/settings.ini";
}

inline std::string get_terminal() {
	return "$TERMINAL --version";
}

inline std::string get_terminal_font() {
	std::string font = "grep --max-count 1 'font_family' ~/.config/kitty/kitty.conf | awk '{$1=\"\";print substr($0,2)}'";
	std::string font_size = "grep --max-count 1 'font_size' ~/.config/kitty/kitty.conf | awk '{$1=\"\";print substr($0,2)}'";
	return "echo \"$("+font+") $("+font_size+")\"";
}

inline std::string get_cpu() {
	return "grep --max-count 1 'model name' /proc/cpuinfo | awk -F ': ' '{print $2}'";
}

inline std::string get_gpu() {
	return "lspci | awk -F ': ' '/VGA compatible controller: /{print $2}'";
}

inline std::string get_memory() {
	std::string percentage = R"(%%)";
	return "free -m | awk '/Mem:/{printf \"%dMiB / %dMiB (%.0f%%%)\", $3, $2, ($3/$2)*100}'";
}

inline std::string get_gpu_driver() {
	return "nvidia-smi --query -d memory | grep --max-count 1 'Driver Version' | awk -F ': ' '{print $2}'";
}

