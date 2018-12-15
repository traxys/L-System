#ifndef FERN_HPP
#define FERN_HPP

#include "lsystem.hpp"

#include <ostream>

namespace fern{
	enum variable { X, F };
	enum constant { p, m, braopen, braclose};

	inline std::ostream& operator<<(std::ostream& os,
									const constant& cst){
		switch(cst){
			case p:
				os << "+";
				break;
			case m:
				os << "-";
				break;
			case braopen:
				os << "[";
				break;
			case braclose:
				os << "]";
				break;
		}
		return os;
	}
	inline std::ostream& operator<<(std::ostream& os,
									const variable& v){
		switch(v){
			case X:
				os << "X";
				break;
			case F:
				os << "F";
				break;
		}
		return os;
	}

	using Fern = LSystem<variable, constant>;
}; // namespace fern

#endif /* FERN_HPP */
