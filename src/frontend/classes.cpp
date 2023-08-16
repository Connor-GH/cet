#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include <cctype>

#include "parsing_error.h"
#include "classes.hpp"

struct visibility {
	std::string str;
	bool pub;
	bool err;
};
static const char *static_file;
static size_t static_line_no;

static const char *const public_identifier_group = "<pub" "lic>:";
static const char *const public_identifier_single = "<pub" "lic>";
static const char *const private_identifier_group = "<pri" "vate>:";
static const char *const private_identifier_single = "<pri" "vate>";

static int is_in_class = 0;


static std::vector<std::string> tokenize_struct(std::string buf,
		std::ifstream& fp) {
	std::vector<std::string> result;
	std::stringstream ss(buf);
	std::string tmp;

	/* Assuming buffer handed contains class identifier at minimum */


	while (getline(ss, tmp, ' ')) {
		result.push_back(tmp);
	}

	while (fp.good() && (buf.rfind("}") == std::string::npos)) {
		getline(fp, buf);

		ss.str("");
		ss.clear();

		/* remove all whitespace */

		for (size_t i = 0; (i < buf.size()) && std::isspace(buf[i]); i++) {
			buf.erase(i,1);
			i--;
		}

		ss << buf;
		while (getline(ss, tmp, ' ')) {
			result.push_back(tmp);
		}
	}
	/* remove all empty array segments */
	for (size_t i = 0; i < result.size(); i++) {
		if (result[i].empty()) {
			result.erase(result.begin() +
			static_cast<std::_Bit_const_iterator::difference_type>(i));
			i--;
		}
	}
	/* tokenize based on semicolons */
	for (size_t i = 0; i < result.size(); i++) {
		if (result[i].rfind(";") != std::string::npos) {
			ss.str("");
			ss.clear();
			ss << result[i];
			while (getline(ss, tmp, ';')) {
				/* remove old symbol */
				result.erase(result.begin() +
				static_cast<std::_Bit_const_iterator::difference_type>(i));

				/* add symbol without semicolon */
				result.insert(result.begin() +
				static_cast<std::_Bit_const_iterator::difference_type>(i),
				tmp);
				/* add semicolon */
				result.insert(result.begin() +
				static_cast<std::_Bit_const_iterator::difference_type>(++i),
				";");
			}
		}
	}
	return result;
}


size_t has_class_identifier(std::string buf, const char *file, size_t line_no) {
/* identifier is split for the sake of bootstrapping */
	static constexpr const char *class_identifier = "<Cla" "ss>";
	static_file = file;
	static_line_no = line_no;
	size_t idx_of_class = 0;
	if (buf.empty())
		return buf.length() + 1;

	idx_of_class = buf.find(class_identifier);
	if (buf.rfind(class_identifier) != idx_of_class) {
		std::string errmsg = "Duplicate ";
		errmsg += class_identifier;
		errmsg += " keyword.";
		parsing_err(ERROR, errmsg.c_str(),
				buf.c_str(), buf.rfind(class_identifier), static_file, static_line_no);
	}

	if (idx_of_class != std::string::npos) {
		/* easy way to get class name */
		is_in_class = 1;
		return idx_of_class + 1;
	}
	return buf.length() + 1;
}


static void vec_check_for_type_errors(std::vector<std::string> vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		while (vec[i] != ";") {
			i++;
		}

		if (vec.at(i-1) != "}") {
			if ((vec.at(i-2) == ";") || (vec.at(i-2) == "{") ||
					(vec.at(i-2).find(public_identifier_single) != std::string::npos)
				|| ((vec.at(i-2) == "const") && ((vec.at(i-3) == ";") \
						|| (vec.at(i-3) == "{") || (vec.at(i-3).find(public_identifier_single) != std::string::npos)\
					   || (vec.at(i-3).find(private_identifier_single) != std::string::npos)  ))
				|| ((vec.at(i-2) == "constexpr") && ((vec.at(i-3) == ";") \
						|| (vec.at(i-3) == "{") || (vec.at(i-2).find(public_identifier_single) != std::string::npos) \
						|| (vec.at(i-3).find(private_identifier_single) != std::string::npos) ))) {

				std::string errbuf = "";
				for (std::string s : vec)
					errbuf += s, errbuf += " ";

				parsing_err(ERROR, "Identifier without type; type explicitly required.",
					errbuf.c_str(), errbuf.find(vec.at(i)),
					static_file, static_line_no);
			}
		}
	}

}
static void vec_find_elem_push_back(std::vector<struct visibility> &vis,
		std::vector<std::string> vec, size_t &i, bool publicity) {
	/* find semicolon. The identifier is before the semicolon. */
	while (vec[i] != ";") {
		i++;
	}

	/* we know for a fact this will not throw an out_of_range */
	struct visibility tmp = {vec.at(i-1), publicity, false };
	vis.push_back(tmp);
}

static std::vector<struct visibility> vec_has_public_identifier(std::vector<std::string> vec) {
	std::vector<struct visibility> vis;
	for (size_t i = 0; i < vec.size(); i++) {
		/* public group is found */
		if (vec[i].find(public_identifier_group) != std::string::npos) {
			/* not private identifier or end of struct */
			while ((vec[i].find("}") == std::string::npos) &&
					(vec[i].find(private_identifier_group) == std::string::npos) &&
						(vec[i].find(private_identifier_single) == std::string::npos)) {
				vec_find_elem_push_back(vis, vec, i, true);
				i++;
			}
		}
		/* private group is found */
		if (vec[i].find(private_identifier_group) != std::string::npos) {
			while ((vec[i].find("}") == std::string::npos) &&
					(vec[i].find(public_identifier_group) == std::string::npos) &&
						(vec[i].find(public_identifier_single) == std::string::npos)) {
				vec_find_elem_push_back(vis, vec, i, false);
				i++;
			}
		}
		if (vec[i].find(private_identifier_single) != std::string::npos) {
			vec_find_elem_push_back(vis, vec, i, false);
		}
		if (vec[i].find(public_identifier_single) != std::string::npos) {
			vec_find_elem_push_back(vis, vec, i, true);
		}
	}
	return vis;
}

void locate_class_name(std::string buf, std::ifstream& fp, const char *file, size_t line_no) {
	std::vector<std::string> vec = tokenize_struct(buf, fp);
	vec_check_for_type_errors(vec);
	std::vector<struct visibility> vis = vec_has_public_identifier(vec);
	for (std::string str : vec)
		std::cerr << str << ", ";
	std::cerr << '\n';

	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i].find("{") != std::string::npos) {
			std::cerr << vec.at(i-1) << std::endl;
		}
	}

	if (!vis.empty())
		for (struct visibility v : vis)
			std::cerr << v.str << " is public: " << ((v.pub) ? "true" : "false") << '\n';
}
