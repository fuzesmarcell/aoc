#include <assert.h>
#include <iostream>

#include <aoc.h>

struct FileEntry {
	bool is_directory;
	std::string name;
	FileEntry* parent;
	
	// file attributes
	size_t size;

	// directory attributes
	std::vector<FileEntry*> children;
};

static FileEntry* make_dir(const char* name, FileEntry* parent) {
	FileEntry* directory = new FileEntry;
	directory->is_directory = true;
	directory->name = name;
	directory->size = 0;
	directory->parent = parent;
	directory->children = {};

	return directory;
}

static FileEntry* make_file(const char* name, FileEntry* parent, size_t size) {
	FileEntry* file = new FileEntry;
	file->is_directory = false;
	file->name = name;
	file->size = size;
	file->parent = parent;
	file->children = {};

	return file;
}

static bool exists(FileEntry* dir, const std::string& name) {
	assert(dir);
	assert(dir->is_directory);

	for (const auto child : dir->children) {
		if (child->name == name) {
			return true;
		}
	}
	
	return false;
}

static void iterate(FileEntry* root, int depth = 0) {
	for (size_t i = 0; i < depth*2; i++)
	{
		std::cout << " ";
	}
	std::cout << "- " << root->name << " ";

	if (root->is_directory) {
		std::cout << "(dir, size=" << root->size << ")\n";
	}
	else {
		std::cout << "(file, size=" << root->size << ")\n";
	}

	for (auto const child : root->children) {
		iterate(child, depth + 1);
	}
}

size_t calc_dir_sizes(FileEntry* node) {
	if (!node->is_directory) {
		return node->size;
	}

	for (auto const child : node->children) {
		node->size += calc_dir_sizes(child);
	}

	return node->size;
}

size_t day_01(FileEntry* node) {
	size_t result = 0;
	if (node->is_directory) {
		if (node->size <= 100000) {
			result += node->size;
		}
	}

	for (auto const child : node->children) {
		result += day_01(child);
	}

	return result;
}

void day_02(FileEntry* node, size_t size, size_t* min, size_t* result) {

	if (node->is_directory) {
		if (node->size >= size) {
			size_t distance = node->size - size;
			if (distance < *min) {
				*min = distance;
				*result = node->size;
			}
		}
	}

	for (auto const child : node->children) {
		day_02(child, size, min, result);
	}
}

int main() {
	std::string content = read_entire_file("input.txt");

	FileEntry* root = nullptr;
	FileEntry* cwd = nullptr;
	char buffer[256];

	// parse directory structure
	for (const auto& line : split(content, "\n")) {
		if (line[0] == '$') { // command
			if (line[2] == 'c' && line[3] == 'd') { // cd
				int result = sscanf(std::string(line).c_str(), "$ cd %s", buffer);
				assert(result == 1);

				std::string arg = buffer;
				if (arg == "/") {
					if (!root) {
						root = make_dir(arg.c_str(), cwd);
					}
					assert(root->name == "/");

					cwd = root;
				}
				else if (arg == "..") {
					assert(cwd);
					cwd = cwd->parent;
				}
				else {
					if (!exists(cwd, arg)) {
						FileEntry* dir = make_dir(arg.c_str(), cwd);
						cwd = dir;
					}
					else {
						FileEntry* next_cwd = nullptr;
						for (const auto child : cwd->children) {
							if (child->name == arg) {
								next_cwd = child;
								break;
							}
						}
						assert(next_cwd);
						cwd = next_cwd;
					}
				}
			}
			else if (line[2] == 'l' && line[3] == 's') { // ls
				// noop
			}
		}
		else if (line[0] == 'd') { // dir name
			int result = sscanf(std::string(line).c_str(), "dir %s", buffer);
			assert(result == 1);
			std::string name = buffer;

			if (!exists(cwd, buffer)) {
				FileEntry* dir = make_dir(buffer, cwd);
				cwd->children.push_back(dir);
			}
		}
		else if (std::isalnum(line[0])) { // size name
			int size;
			int result = sscanf(std::string(line).c_str(), "%d %s", &size, buffer);
			assert(result == 2);
			std::string name = buffer;

			if (!exists(cwd, buffer)) {
				FileEntry* file = make_file(buffer, cwd, size);
				cwd->children.push_back(file);
			}
		}
		else {
			assert(false);
		}
	}

	size_t used_space = calc_dir_sizes(root);
	//iterate(root, 0);

	std::cout << "Part 01: " << day_01(root) << "\n";
	
	size_t total_disk_space = 70000000;
	size_t space_needed = 30000000;

	assert(used_space <= total_disk_space);
	size_t unused_space = total_disk_space - used_space;
	size_t requires_space = space_needed - unused_space;

	size_t result;
	size_t min = std::numeric_limits<size_t>::max();
	day_02(root, requires_space, &min, &result);
	
	std::cout << "Part 02: " << result << "\n";
}