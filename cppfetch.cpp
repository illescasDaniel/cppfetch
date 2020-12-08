#include <cstddef>
#include <fstream>
#include <iostream>
#include <future>
#include <cstdlib>
#include <cstring>

#include <array>
#include <memory>
#include <string>
#include <iomanip>

// system functions
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>

#include <chrono>
#include <cmath>

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

void print_empty_line();
future<void> print_colors_2();
future<void> print_full_name();
future<void> print_os_name_2();

/// system memory. Units are in Kibibytes
struct sys_memory { 
	int used, total;
	double used_percentage;
};
future<sys_memory> get_system_memory_async();
void print_system_memory(sys_memory&& memory);

struct uptime_data { double hours, minutes, seconds; };
future<uptime_data> get_uptime_async();
void print_uptime(uptime_data&& uptime);

future<std::string> get_desktop_env_async();
void print_desktop_env(std::string&& desktop_env);

future<std::string> get_os_name_async();
void print_os_name(std::string&& os_name);

future<std::string> get_kernel_version_async();
void print_kernel_version(std::string&& kernel_version);


const std::string left_padding = "  ";

int main(int argc, char* argv[]) {

	// print_os_name();
	// print_empty_line();
	print_colors_2().wait();
	print_empty_line();
	print_full_name().wait();
	std::cout << left_padding << "----------------------" << endl;

	auto system_memory = get_system_memory_async();
	auto uptime = get_uptime_async();
	auto desktop_env = get_desktop_env_async();
	auto kernel_version = get_kernel_version_async();
	auto os_name = get_os_name_async();
	
	//color_print("OS", get_full_os_name());
	//color_print("Kernel", get_kernel_version());
	//color_print2("Uptime", get_uptime());
	//auto shell_name = color_print2("Shell", get_shell());
	//auto packages = color_print("Packages", get_pm_packages());
	//auto resolution = color_print("Resolution", get_resolution());
	//color_print("Desktop Environment", get_desktop_env());
	//auto window_manager = color_print("Window Manager", get_wm());
	//auto theme = color_print("Theme", get_theme());
	//auto icons = color_print("Icons", get_icons());
	//auto cursor = color_print("Cursor", get_cursor());
	//auto font = color_print("Font", get_font());
	//auto terminal = color_print("Terminal", get_terminal());
	//auto terminal_font = color_print("Terminal Font", get_terminal_font());
	//auto cpu = color_print("CPU", get_cpu());
	//auto gpu = color_print("GPU", get_gpu());
	//color_print("Memory", get_memory());
	//auto gpu_driver = color_print("Nvidia GPU Driver", get_gpu_driver());
	
	// os_name.get();
	// use get to get result, wait to wait to finish
	

	print_os_name(os_name.get());
	// host
	print_kernel_version(kernel_version.get());
	print_uptime(uptime.get());
	// packages
	// shell
	// resolution
	print_desktop_env(desktop_env.get());
	// wm
	// theme
	// icons
	// font
	// terminal
	// terminal font
	// cpu
	// gpu
	print_system_memory(system_memory.get());
	// gpu driver
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

inline std::string get_accent_color() {
	return "\033[0;32m";
}

inline std::string get_normal_color() {
	return "\033[0m";
}

inline future<int> color_print(std::string&& colored_text, std::string&& normal_text) {
	std::string final_command = "echo \""+left_padding+get_accent_color()+colored_text+get_normal_color()+": $("+normal_text+")\"";
	return system_call_str(final_command);
}

inline future<int> color_print2(std::string colored_text, std::string normal_text) {
	return std::async(std::launch::async, [&] {
		const auto command_output = exec(normal_text.c_str());//strdup(normal_text.c_str());
		std::cout << left_padding << colored_text << ": " << trim(command_output) << std::endl << std::flush;
		return 0;
	});
}

inline void color_print3(std::string colored_text, std::string normal_text) {
	std::cout << left_padding << get_accent_color() << colored_text << ": " << get_normal_color() << trim(normal_text) << std::endl;
}

//

// inline int print_figlet_os_name() {
// 	return sync_system_call_str(std::string("/usr/bin/figlet ")+std::string("$("+get_os_name()+")"));
// }

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

std::string get_kernel_version()
{
	struct utsname name;
	uname(&name);
	return name.release;
}

future<std::string> get_kernel_version_async()
{
	return std::async(std::launch::async, get_kernel_version);
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
inline future<std::string> get_os_name_async() 
{
	return std::async(std::launch::async, get_os_name2);
}



inline sys_memory get_system_memory()
{
	ifstream stream = ifstream();

	stream.open("/proc/meminfo");
	if (!stream.fail()) {
		std::string line = "";
		std::string mem_free = "";
		std::string mem_total = "";
		std::string buffers_str = "";
		std::string cached_str = "";
		std::string total_swap_str = "";
		std::string swap_free_str = "";
		std::string sreclaimable_str = "";
		bool found = false;
		bool found_mem_free = false;
		bool found_mem_total = false;
		bool found_buffers = false;
		bool found_cached = false;
		bool found_total_swap = false;
		bool found_free_swap = false;
		bool found_sreclaimable = false;

		while(std::getline(stream, line) && !found) {	
			if (line.find("MemFree:") != std::string::npos) {
				found_mem_free = true;
				mem_free = line;
			}
			if (line.find("MemTotal:") != std::string::npos) {
				found_mem_total = true;
				mem_total = line;
			}
			if (line.find("Buffers:") != std::string::npos) {
				found_buffers = true;
				buffers_str = line;
			}
			if (line.find("Cached:") != std::string::npos && line.find("SwapCached:") == std::string::npos) {
				found_cached = true;
				cached_str = line;
			}
			if (line.find("SwapTotal:") != std::string::npos) {
				found_total_swap = true;
				total_swap_str = line;
			}
			if (line.find("SwapFree:") != std::string::npos) {
				found_free_swap = true;
				swap_free_str = line;
			}
			if (line.find("SReclaimable:") != std::string::npos) {
				found_sreclaimable = true;
				sreclaimable_str = line;
			}

			found = found_mem_free && found_mem_total && found_buffers && found_cached && found_total_swap && found_free_swap && found_sreclaimable;
		}

		if (found) {
			
			const size_t mem_free_colon_pos = 7;
			const size_t mem_total_colon_pos = 8;
			const size_t buffers_colon_pos = 7;
			const size_t swap_total_colon_pos = 9;
			const size_t cached_colon_pos = 6;
			const size_t swap_free_colon_pos = 8;
			const size_t sreclaimable_colon_pos = 12;

			// we don't really need these positions, but we need to make sure that the strings end in exactly "kB"
			const size_t kB_position_mem_free = mem_free.find("kB", mem_free_colon_pos);
			const size_t kB_position_mem_total = mem_total.find("kB", mem_total_colon_pos);
			const size_t kB_position_buffers = buffers_str.find("kB", buffers_colon_pos);
			const size_t kB_position_swap_total = total_swap_str.find("kB", swap_total_colon_pos);
			const size_t kB_position_swap_free = swap_free_str.find("kB", swap_free_colon_pos);
			const size_t kB_position_cached = cached_str.find("kB", cached_colon_pos);
			const size_t kB_position_sreclaimable = sreclaimable_str.find("kB", sreclaimable_colon_pos);

			if (kB_position_mem_free != std::string::npos 
					&& kB_position_mem_total != std::string::npos
					&& kB_position_buffers != std::string::npos
					&& kB_position_swap_total != std::string::npos
					&& kB_position_swap_free != std::string::npos
					&& kB_position_sreclaimable != std::string::npos
					&& kB_position_cached != std::string::npos) {

				const std::string mem_free_kB = trim(std::string(mem_free.begin() + mem_free_colon_pos + 1, mem_free.end() - 2)); // 2 = size of kB
				const std::string mem_total_kB = trim(std::string(mem_total.begin() + mem_total_colon_pos + 1, mem_total.end() - 2));
				const std::string swap_total_kB = trim(std::string(total_swap_str.begin() + swap_total_colon_pos + 1, total_swap_str.end() - 2));
				const std::string swap_free_kB = trim(std::string(swap_free_str.begin() + swap_free_colon_pos + 1, swap_free_str.end() - 2));
				const std::string buffers_kB = trim(std::string(buffers_str.begin() + buffers_colon_pos + 1, buffers_str.end() - 2));
				const std::string sreclaimable_kB = trim(std::string(sreclaimable_str.begin() + sreclaimable_colon_pos + 1, sreclaimable_str.end() - 2));
				const std::string cached_kB = trim(std::string(cached_str.begin() + cached_colon_pos + 1, cached_str.end() - 2));

				// based on the "free" system command
				
				const int free_memory = stoi(mem_free_kB);
				const int swap_free = stoi(swap_free_kB);
				const int free = free_memory + swap_free;

				const int total_memory = stoi(mem_total_kB);
				const int swap_total = stoi(swap_total_kB);
				const int total = total_memory + swap_total;

				const int buffers = stoi(buffers_kB);

				const int cached = stoi(cached_kB);
				const int sreclaimable = stoi(sreclaimable_kB);
				const int cache = cached + sreclaimable;

				const int used_memory = total - free - buffers - cache;
				const double free_percentage = free_memory == 0 or total_memory == 0 ? 0 : double(used_memory)/double(total_memory);

				return { .used=used_memory, .total=total_memory, .used_percentage=free_percentage };
			}
		}
	}

	stream.close();
	return { 0, 0, 0 };
}

inline future<sys_memory> get_system_memory_async() {
	return std::async(std::launch::async, get_system_memory);
}

uptime_data get_uptime()
{
	std::chrono::milliseconds uptime(0u);
	double uptime_seconds;
	
	if (std::ifstream("/proc/uptime", std::ios::in) >> uptime_seconds)
	{
		uptime = std::chrono::seconds(static_cast<unsigned long long>(uptime_seconds*1000.0));
	}
	
	const double hours = uptime_seconds / 3600;
	const double truncated_hours = trunc(hours);
	const double minutes = (hours - truncated_hours) * 60;
	const double truncated_minutes = trunc(minutes);
	const double seconds = (minutes - truncated_minutes) * 60;
	const double truncated_seconds = trunc(seconds);

	return { truncated_hours, truncated_minutes, truncated_seconds };
}	

std::string get_desktop_env() {
	return getenv("XDG_CURRENT_DESKTOP");
}
future<std::string> get_desktop_env_async() {
	return std::async(std::launch::async, get_desktop_env);
}

future<uptime_data> get_uptime_async()
{
	return std::async(std::launch::async, get_uptime);
}




inline void print_os_name(std::string&& os_name) 
{
	color_print3("OS", os_name);
}

inline void print_kernel_version(std::string&& kernel_version) 
{
	color_print3("Kernel", kernel_version);
}

inline void print_desktop_env(std::string&& desktop_env) 
{
	color_print3("Desktop Environment", desktop_env);
}

inline future<void> print_full_name() 
{
	return std::async(std::launch::async, []{
		std::cout << left_padding << get_accent_color() << get_user_name() << get_normal_color() << '@' << get_accent_color() << get_user_node_name() << get_normal_color() << std::endl << std::flush;
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

inline void print_uptime(uptime_data&& uptime)
{
	cout << left_padding << get_accent_color() << "Uptime:" << get_normal_color() << ' ' 
	<< uptime.hours << " hours" << ' '
	<< uptime.minutes << " minutes" << ' '
	<< uptime.seconds << " seconds"
	<< std::endl;
}	

inline void print_system_memory(sys_memory&& memory)
{
	cout << std::fixed << std::setprecision(1) << left_padding << get_accent_color() << "Memory:" << get_normal_color() << ' '
	<< memory.used / 1024 << " MiB" << " / "
	<< memory.total / 1024 << " MiB" << ' '
	<< '(' << memory.used_percentage * 100 << "%)"
	<< std::endl;
}





inline std::string get_full_os_name() {
	std::string os_name = "$(cat /etc/*-release | head -n 1)";
	std::string architecture = "$(uname --machine)";
	return "echo \""+os_name+" "+architecture+"\"";
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

