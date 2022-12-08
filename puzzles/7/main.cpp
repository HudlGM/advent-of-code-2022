#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

const std::string DIR_ROOT{"/"};
const std::string DIR_PARENT{".."};
const size_t ONE_HUNDRED_THOUSAND{100000};
const size_t TOTAL_DISK_SPACE{70000000};
const size_t SPACE_REQUIRED_FOR_UPDATE{30000000};

class File
{
public:
  File(const std::string &name_, const size_t &size_)
    : name{name_}
    , size{size_}
  {
  }

  ~File()
  {
  }

  const std::string name;
  const size_t size;
};

class Directory
{
public:
  Directory(const std::string &name_, Directory *parent_ = NULL)
    : name{name_}
    , _parent{parent_}
  {
  }

  ~Directory()
  {
  }

  Directory *parent() const
  {
    return _parent;
  }

  Directory *childDir(const std::string &withName)
  {
    for (Directory &d : _dirs) {
      if (d.name == withName) {
        return &d;
      }
    }

    return NULL;
  }

  size_t size() const
  {
    size_t total = 0;

    for (const Directory &d : _dirs) {
      total += d.size();
    }

    for (const File &f : _files) {
      total += f.size;
    }

    return total;
  }

  void addDirectory(const Directory &d)
  {
#ifdef VERBOSE
    std::cout << "Adding directory " << d.name << " to " << name << std::endl;
#endif
    _dirs.push_back(d);
  }

  void addFile(const File &f)
  {
#ifdef VERBOSE
    std::cout << "Adding file " << f.name << " with size " << f.size << " to " << name << std::endl;
#endif
    _files.push_back(f);
  }

  const std::string name;

  static size_t sumOfDirsLargerThan(const size_t &largestSize, const Directory& top)
  {
    size_t total = 0;

    if (top.size() <= ONE_HUNDRED_THOUSAND) {
      total += top.size();
    }

    for (const Directory &d : top._dirs) {
      total += sumOfDirsLargerThan(largestSize, d);
    }

    return total;
  }

  static std::vector<size_t> dirSizes(const Directory &top)
  {
    std::vector<size_t> result = { top.size() };

    for (const Directory &d : top._dirs) {
      const std::vector<size_t> sizes = dirSizes(d);
      for (const size_t &size : sizes) {
        result.push_back(size);
      }
    }

    return result;
  }

  static size_t smallestDirLargerThan(const size_t &minimumSize, const Directory& top)
  {
    std::vector<size_t> sizes = dirSizes(top);

    std::sort(sizes.begin(), sizes.end());

    for (const size_t &size : sizes) {
      if (size >= minimumSize) {
        return size;
      }
    }

    return 0;
  }

private:
  Directory *_parent;
  std::vector<Directory> _dirs;
  std::vector<File> _files;
};

int main()
{
  std::ifstream in("puzzles/7/input.txt");

  Directory root(DIR_ROOT);

  Directory *current = &root;

  std::string l; // line

  std::getline(in, l);

  while (in) {
    if (l.at(0) == '$') {
      // command
      const std::string command = l.substr(2);
      if (command.find("cd") == 0) {
        const std::string dir = command.substr(3);
        if (dir == DIR_ROOT) {
          current = &root;
        } else if (dir == DIR_PARENT) {
          current = current->parent();
        } else {
          current = current->childDir(dir);
        }
      } else if (l == "ls") {
        // no-op
      }
    } else if (l.at(0) == 'd') {
      // directory
      current->addDirectory(Directory(l.substr(4), current));
    } else {
      // file
      const size_t space = l.find(' ');
      current->addFile(File(l.substr(space+1), std::stoul(l.substr(0, space))));
    }

    std::getline(in, l);
  }

  std::cout << "Sum of dirs larger than " << ONE_HUNDRED_THOUSAND << ": " << Directory::sumOfDirsLargerThan(ONE_HUNDRED_THOUSAND, root) << std::endl;

  std::cout << "Size of smallest directory which would allow update to complete: " << Directory::smallestDirLargerThan(SPACE_REQUIRED_FOR_UPDATE - (TOTAL_DISK_SPACE - root.size()), root) << std::endl;

  return 0;
}