#include <iostream>
#include <future>
#include <cstdlib>
#include <cstring>

using namespace std;

inline int sync_system_call_str(std::string&& command);
inline int sync_system_call_str(std::string& command);
inline future<int> system_call(const char* command);
inline future<int> system_call_str(std::string& command);
inline future<int> system_call_str(std::string&& command);
inline future<int> color_print(std::string&& colored_text, std::string&& normal_text);

inline int print_newline();
inline int print_os_name();
inline int print_colors();
inline future<int> print_name();

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
	print_colors();
	print_name().wait();
	
	auto os_name = color_print("OS", get_full_os_name());
	auto kernel_version = color_print("Kernel", get_kernel_version());
	auto uptime = color_print("Uptime", get_uptime());
	auto shell_name = color_print("Shell", get_shell());
	auto packages = color_print("Packages", get_pm_packages());
	auto resolution = color_print("Resolution", get_resolution());
	auto desktop_env = color_print("Desktop Environment", get_desktop_env());
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

inline future<int> system_call(const char* command) {
	return std::async(std::launch::async, system, command);
}

inline future<int> system_call_str(std::string& command) {
	
	int len = command.size();
	char *c = new char[len + 1];
	std::copy(command.begin(), command.end(), c);
	c[len] = '\0';

	return std::async(std::launch::async, system, c);
}

inline future<int> system_call_str(std::string&& command) {
	
	int len = command.size();
	char *c = new char[len + 1];
	std::copy(command.begin(), command.end(), c);
	c[len] = '\0';

	return std::async(std::launch::async, system, c);
}

inline int sync_system_call_str(std::string&& command) {
	
	int len = command.size();
	char *c = new char[len + 1];
	std::copy(command.begin(), command.end(), c);
	c[len] = '\0';

	return system(c);
}

inline int sync_system_call_str(std::string& command) {
	
	int len = command.size();
	char *c = new char[len + 1];
	std::copy(command.begin(), command.end(), c);
	c[len] = '\0';

	return system(c);
}

inline future<int> color_print(std::string&& colored_text, std::string&& normal_text) {
	std::string green = "\033[0;32m";
	std::string normal = "\033[0m";
	std::string final_command = "echo \""+left_padding+green+colored_text+normal+": $("+normal_text+")\"";
	
	return system_call_str(final_command);
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

