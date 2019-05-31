/**
 * ------------------------------------------------------------------------------------------------
 * File:   Variable.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include "SVariable.h"
/**
 * std
 */
using namespace std;
/**
 * ------------------------------------------------------------------------------------------------
 * Clone Variable
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Clone(Var var, false_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		
		for (auto& v : Var::Map(var)) {
			m[v.first] = Clone(Var(v.second), false_type(), false_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Clone(v, false_type(), false_type()));
		}
		return Obj(move(l));
	}
	return var;
}
Var Var::Clone(Var var, true_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Clone(v.second, true_type(), false_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Clone(v, true_type(), false_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsLink(var)) {
		return Obj::Link(
			Clone(Var::Link(var), true_type(), false_type())
		);
	}
	return var;
}
Var Var::Clone(Var var, false_type, true_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Clone(v.second, false_type(), true_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Clone(v, false_type(), true_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsString(var)) {
		return Obj(Var::String(var));
	}
	if (Var::IsInteger(var)) {
		return Obj(Var::Integer(var));
	}
	if (Var::IsFloat(var)) {
		return Obj(Var::Float(var));
	}
	if (Var::IsBoolean(var)) {
		return Obj(Var::Boolean(var));
	}
	if (Var::IsBuffer(var)) {
		return Obj(Var::Buffer(var));
	}
	if (Var::IsLink(var)) {
		return Obj::Link(Var::Link(var));
	}
	return Obj(nullptr);
}
Var Var::Clone(Var var, true_type, true_type) {	
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Clone(v.second, true_type(), true_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Clone(v, true_type(), true_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsLink(var)) {
		return Obj::Link(
			Clone(Var::Link(var), true_type(), true_type())
		);
	}
	if (Var::IsString(var)) {
		return Obj(Var::String(var));
	}
	if (Var::IsInteger(var)) {
		return Obj(Var::Integer(var));
	}
	if (Var::IsFloat(var)) {
		return Obj(Var::Float(var));
	}
	if (Var::IsBoolean(var)) {
		return Obj(Var::Boolean(var));
	}
	if (Var::IsBuffer(var)) {
		return Obj(Var::Buffer(var));
	}
	return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Copy Variables
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Copy(Var var, true_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Copy(v.second, true_type(), false_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Copy(v, true_type(), false_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsLink(var)) {
		return Copy(Var::Link(var), true_type(), false_type());
	}
	return var;
}

Var Var::Copy(Var var, false_type, true_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Copy(v.second, false_type(), true_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Copy(v, false_type(), true_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsString(var)) {
		return Obj(Var::String(var));
	}
	if (Var::IsInteger(var)) {
		return Obj(Var::Integer(var));
	}
	if (Var::IsFloat(var)) {
		return Obj(Var::Float(var));
	}
	if (Var::IsBoolean(var)) {
		return Obj(Var::Boolean(var));
	}
	if (Var::IsBuffer(var)) {
		return Obj(Var::Buffer(var));
	}
	return Obj(nullptr);
}
Var Var::Copy(Var var, true_type, true_type) {	
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Copy(v.second, true_type(), true_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Copy(v, true_type(), true_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsLink(var)) {
		return Copy(Var::Link(var), true_type(), true_type());
	}
	if (Var::IsString(var)) {
		return Obj(Var::String(var));
	}
	if (Var::IsInteger(var)) {
		return Obj(Var::Integer(var));
	}
	if (Var::IsFloat(var)) {
		return Obj(Var::Float(var));
	}
	if (Var::IsBoolean(var)) {
		return Obj(Var::Boolean(var));
	}
	if (Var::IsBuffer(var)) {
		return Obj(Var::Buffer(var));
	}
	return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Strip Variables
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Strip(Var var, false_type, false_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end(); ++it) {
			it->second = Strip(it->second, false_type(), false_type());
		}
		return var;
	}
	if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end(); ++it) {
			*it = Strip(*it, false_type(), false_type());
		}
		return var;
	}
	return Obj(nullptr);
}
Var Var::Strip(Var var, false_type, true_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end(); ++it) {
			it->second = Strip(it->second, false_type(), true_type());
		}
		return var;
	}
	if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end(); ++it) {
			*it = Strip(*it, false_type(), true_type());
		}
		return var;
	}
	if (Var::IsLink(var)) {
		Var::Link(var) = Strip(Var::Link(var), false_type(), true_type());
		return var;
	}
	return Obj(nullptr);
}
Var Var::Strip(Var var, true_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Strip(v.second, true_type(), false_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Strip(v, true_type(), false_type()));
		}
		return Obj(move(l));
	}
	return Obj(nullptr);
}
Var Var::Strip(Var var, true_type, true_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			m[v.first] = Strip(v.second, true_type(), true_type());
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			l.push_back(Strip(v, true_type(), true_type()));
		}
		return Obj(move(l));
	}
	if (Var::IsLink(var)) {
		return Obj::Link(Strip(Var::Link(var), true_type(), true_type()));
	}
	return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Trim Variables
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Trim(Var var, false_type, false_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end();) {
			it->second = Trim(it->second, false_type(), false_type());
			// check 
			if (Var::IsUndefined(it->second)) {
				it = m.erase(it);
				continue;
			}
			++it;
		}
		if (m.empty()) {
			return Obj(nullptr);
		}
	} else if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end();) {
			*it = Trim(*it, false_type(), false_type());
			// check
			if (Var::IsUndefined(*it)) {
				it = l.erase(it);
				continue;
			}
			++it;
		}
		if (l.empty()) {
			return Obj(nullptr);
		}
	}
	return var;
}
Var Var::Trim(Var var, false_type, true_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end();) {
			it->second = Trim(it->second, false_type(), true_type());
			// check
			if (Var::IsUndefined(it->second)) {
				it = m.erase(it);
				continue;
			}
			++it;
		}
		if (m.empty()) {
			return Obj(nullptr);
		}
	} else if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end();) {
			*it = Trim(*it, false_type(), true_type());
			// check
			if (Var::IsUndefined(*it)) {
				it = l.erase(it);
				continue;
			}
			++it;
		}
		if (l.empty()) {
			return Obj(nullptr);
		}
	}
	if (Var::IsLink(var)) {
		Var aux = Trim(var, false_type(), true_type());
		if (Var::IsUndefined(aux)) {
			return Obj(nullptr);
		}
		Var::Link(var) = aux;
	}
	return var;
}
Var Var::Trim(Var var, true_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Trim(v.second, true_type(), false_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return m.empty() ? Obj(nullptr) : Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			Var aux = Trim(v, true_type(), false_type());
			if (Var::IsDefined(aux)) {
				l.push_back(aux);
			}

		}
		return l.empty() ? Obj(nullptr) : Obj(move(l));
	}
	return var;
}
Var Var::Trim(Var var, true_type, true_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Trim(v.second, true_type(), true_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return m.empty() ? Obj(nullptr) : Obj(move(m));
	}
	if (Var::IsList(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Trim(v.second, true_type(), true_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return m.empty() ? Obj(nullptr) : Obj(move(m));
	}
	if (Var::IsLink(var)) {
		Var aux = Trim(var, true_type(), true_type());
		if (Var::IsUndefined(aux)) {
			return Obj(nullptr);
		}
		return Obj::Link(aux);
	}
	return var;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Struct Variables
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Shape(Var var, false_type, false_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end();) {
			it->second = Shape(it->second, false_type(), false_type());
			// check
			if (Var::IsUndefined(it->second)) {
				it = m.erase(it);
				continue;
			}
			++it;
		}
		return var;
	}
	if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end();) {
			*it = Shape(*it, false_type(), false_type());
			// check
			if (Var::IsUndefined(*it)) {
				it = l.erase(it);
				continue;
			}
			++it;
		}
		return var;
	}
	return Obj(nullptr);
}
Var Var::Shape(Var var, false_type, true_type) {
	if (Var::IsMap(var)) {
		::Map& m = Var::Map(var);
		for (Map::iterator it = m.begin(); it != m.end();) {
			it->second = Shape(it->second, false_type(), true_type());
			// check
			if (Var::IsUndefined(it->second)) {
				it = m.erase(it);
				continue;
			}
			++it;
		}
		return var;
	}
	if (Var::IsList(var)) {
		::List& l = Var::List(var);
		for (List::iterator it = l.begin(); it != l.end();) {
			*it = Shape(*it, false_type(), true_type());
			// check
			if (Var::IsUndefined(*it)) {
				it = l.erase(it);
				continue;
			}
			++it;
		}
		return var;
	}
	if (Var::IsLink(var)) {
		Var aux = Shape(var, false_type(), true_type());
		if (Var::IsDefined(aux)) {
			return Var::Link(var) = aux;
		}
	}
	return Obj(nullptr);
}
Var Var::Shape(Var var, true_type, false_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Shape(v.second, true_type(), false_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::List l;
		for (auto& v : Var::List(var)) {
			Var aux = Shape(v, true_type(), false_type());
			if (Var::IsDefined(aux)) {
				l.push_back(aux);
			}

		}
		return Obj(move(l));
	}
	return Obj(nullptr);
}
Var Var::Shape(Var var, true_type, true_type) {
	if (Var::IsMap(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Shape(v.second, true_type(), true_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return Obj(move(m));
	}
	if (Var::IsList(var)) {
		::Map m;
		for (auto& v : Var::Map(var)) {
			Var aux = Shape(v.second, true_type(), true_type());
			if (Var::IsDefined(aux)) {
				m[v.first] = aux;
			}
		}
		return Obj(move(m));
	}
	if (Var::IsLink(var)) {
		Var aux = Shape(var, true_type(), true_type());
		if (Var::IsDefined(aux)) {
			return Obj::Link(aux);
		}
	}
	return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Join Variables
 * ------------------------------------------------------------------------------------------------
 */
Var Var::Join(Var var) {
	if (Var::IsMap(var)) {
		for (auto& v : Var::Map(var)) {
			v.second = Join(v.second);
		}
		return var;
	}
	if (Var::IsList(var)) {
		for (auto& v : Var::List(var)) {
			v = Join(v);
		}
		return var;
	}
	if (Var::IsLink(var)) {
		return Var::Link(var);
	}
	return var;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
