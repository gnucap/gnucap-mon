/*                             -*- C++ -*-
 * Copyright (C) 2016 Felix Salfelder
 * Author: same
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "u_probe.h"
#include "u_function.h"
#include "globals.h"
#include "e_card.h"
#include "e_cardlist.h"
#include "e_node.h"

// -uf compat hacks
#ifndef _U_FUNC
typedef std::string fun_t;
#define CKT_NODE NODE
#define to_fun_t to_string
#endif
#ifndef HAVE_UINT_T
typedef int uint_t;
#endif
namespace {
class probe : public FUNCTION {
public:
	probe() : FUNCTION()
	{ untested();
	}
public:
	// this is essentially add_branches...
	PROBE* find(const std::string& device,
			const std::string& param,
			const CARD_LIST* scope) const
	{ untested();
		trace2("find", device, param);
		assert(scope);
		PROBE* found_something = NULL;

		std::string::size_type dotplace = device.find_first_of(".");
		if (dotplace != std::string::npos) { untested();
			std::string dev = device.substr(dotplace+1, std::string::npos);
			std::string container = device.substr(0, dotplace);
			for (CARD_LIST::const_iterator
					i = scope->begin();  i != scope->end();  ++i) { untested();
				CARD* card = *i;
				if (card->is_device()
						&& card->subckt()
						&& wmatch(card->short_label(), container)) { untested();
					trace3("PROBELIST::add_branches dot cont:", container, dev,
							card->long_label());
					found_something = find(dev, param, card->subckt());
				}else{ untested();
				}
			}
		}else{ untested();
#if UF_NODEPROBE
			trace1("PROBELIST::add_branches looking up node", device );
			CKT_NODE* node = dynamic_cast<CKT_NODE*>((*scope).node(device));
			if (node) { untested();
				found_something = new PROBE(param, node);
			}else{
				trace1("PROBELIST::add_branches not found", device);
			}
#endif
			{ //components
				CARD_LIST::const_iterator i = scope->find_(device);
				if (i != scope->end()) { untested();
					if (param=="V?") {  untested();
						for(uint_t ip=1; ip<=(*i)->net_nodes(); ip++) {
							char str[10];
							sprintf(str,"V%1d",ip);
							std::string paramipn(str);
							found_something = new PROBE(paramipn, *i);
						}
					}else { untested();
						trace1("hmm", param);
						found_something = new PROBE(param, *i);
					}
				}else{ untested();
					trace1("device not found", device);
				}
			}
		}
		return found_something;
	}
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{ untested();
		std::string what(Cmd.ctos(TOKENTERM));/* parameter */
		std::string where;
		int paren=0;
		if( ( paren = Cmd.skip1b('(')) ) { untested();
			where=Cmd.ctos();
		}else{ incomplete();
		}
		trace3("probing", what, Cmd.tail(), where);
		paren -= Cmd.skip1b(')');
		if (paren != 0) { untested();
			Cmd.warn(bWARNING, "need )");
		}else{ untested();
		}
		PROBE* p = find(where, what, Scope);

		if(!p){
			p = find(where, what, &CARD_LIST::card_list);
		}

		if(p){ untested();
			return to_fun_t(p->value());
		}else{ untested();
			return to_fun_t(NOT_VALID);
		}
	}
} p_probe;
DISPATCHER<FUNCTION>::INSTALL d_probe(&function_dispatcher, "probe", &p_probe);
/*--------------------------------------------------------------------------*/
}
