#include <iostream>

#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/version.hpp>
#include <fstream>

namespace libt = libtorrent;

void printDictEntry(std::ostream& os, const libt::lazy_entry& e, int level);
void printListEntry(std::ostream& os, const libt::lazy_entry& e, int level);

bool entryShouldBeIgnored(const std::string& name) {
	if (name == "info-hash" ||
		name == "peers" ||
		name == "peers6" ||
		name == "piece_priority" ||
		name == "pieces" ||
		name == "bitmask") {
		return true;
	}
	return false;
}

void printEntry(std::ostream& os, const libt::lazy_entry& e, int level) {
	switch (e.type()) {
		case libt::lazy_entry::none_t:
			break;
		case libt::lazy_entry::dict_t:
			printDictEntry(os, e, level+1);
			break;
		case libt::lazy_entry::list_t:
			printListEntry(os, e, level+1);
			break;
		case libt::lazy_entry::string_t:
			os << e.string_value();
			break;
		case libt::lazy_entry::int_t:
			os << e.int_value();
			break;
	}
}

void printDictEntry(std::ostream& os, const libt::lazy_entry& e, int level)
{
	for (int i = 0; i < e.dict_size(); ++i) {
		auto p = e.dict_at(i);
		if (entryShouldBeIgnored(p.first)) {
			continue;
		}
		for (int j = 0; j < level; ++j) {
			os << '\t';
		}
		os << p.first << ":\t";
		printEntry(os, *p.second, level + 1);
		os << std::endl;
	}
}

void printListEntry(std::ostream& os, const libt::lazy_entry& e, int level)
{
	os << '[' << std::endl;
	for (int j = 0; j < level; ++j) {
		os << '\t';
	}
	for (int i = 0; i< e.list_size(); ++i) {
		printEntry(os, *e.list_at(i), level+1);
		os << ", ";
	}
	for (int j = 0; j < level; ++j) {
		os << '\t';
	}
	os << ']';
}

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
		return 2;
	}
	std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		libt::error_code ec;
#if LIBTORRENT_VERSION_NUM < 10100
		libt::lazy_entry fast;
		libt::lazy_bdecode(buffer.data(), buffer.data() + buffer.size(), fast, ec);
		if (ec) {
			std::cerr << "Error decoding file" << std::endl;
			return 3;
		}
#else
		libt::bdecode_node fast;
		libt::bdecode(data.constData(), data.constData() + data.size(), fast, ec);
		if (ec || (fast.type() != libt::bdecode_node::dict_t)) return false;
#endif
		printEntry(std::cout, fast, -1);

	}
    return 0;
}
