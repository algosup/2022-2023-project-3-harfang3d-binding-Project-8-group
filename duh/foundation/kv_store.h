// Key-value store

#include <map>
#include <string>
#include <vector>

namespace hg {

class KeyValueStore {
public:
	bool Open(const std::string &path);
	bool Save(const std::string &path);

	bool Get(const std::string &key, std::string &value) const;
	bool Set(const std::string &key, const std::string &value);

	std::vector<std::string> FindValue(const std::string &value) const;

	void PushPrefix(const std::string &prefix);
	void PopPrefix();

	void Close();

private:
	std::map<std::string, std::string> kvs;

	void CommitPrefix();

	std::vector<std::string> prefix_stack;
	std::string prefix;
};

} // namespace hg
