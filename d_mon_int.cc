/* (c) Felix Salfelder 2016
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
 *------------------------------------------------------------------
 * integrate an expression
 */
#include "e_elemnt.h"
#include "globals.h"
#ifndef HAVE_UINT_T
typedef int uint_t;
#endif
/*--------------------------------------------------------------------------*/
namespace { //
/*--------------------------------------------------------------------------*/
class DEV_INT : public ELEMENT { //
private:
	double _m, _v1;
protected:
	explicit DEV_INT(const DEV_INT& p) :ELEMENT(p) {}
public:
	explicit DEV_INT()	:ELEMENT() {}
protected: // override virtual
	std::string value_name()const {return "int";}
	std::string dev_type()const	{return "int";}
	uint_t	   max_nodes()const	{return 0;}
	uint_t	   min_nodes()const	{return 0;}
	uint_t	   matrix_nodes()const	{return 0;}
	uint_t	   net_nodes()const	{return 0;}
	//bool	   has_iv_probe()const  {return true;}
	bool	   use_obsolete_callback_parse()const {return false;}
	CARD*	   clone()const		{return new DEV_INT(*this);}
	void     precalc_last();
	double tr_involts()const
	{ untested();
		return tr_involts_limited();
	}
	double tr_involts_limited()const
	{ untested();
		_value.e_val(NOT_INPUT, &CARD_LIST::card_list);
		return _value;
	}
	COMPLEX  ac_involts()const	{untested();return ac_outvolts();}
	virtual void tr_iwant_matrix(){}
	virtual void ac_iwant_matrix(){}
	std::string port_name(uint_t)const { unreachable();
		return "";
	}
private:
	double _last;

	public:// overrides
	bool tr_needs_eval()const{return true;}
	double tr_probe_num(const std::string& )const;
	//		void tr_begin(){q_eval();}
	void tr_begin(){q_accept(); _m = _v1 = _last =0.;}
	bool do_tr(){
		q_accept();
		return ELEMENT::do_tr(); // true.
	}
	void tr_accept(){
		ELEMENT::tr_accept();
		if(_sim->is_initial_step()){ untested();
			_m = 0;
			_v1 = tr_involts();
		} else if( _last<_sim->_time0){
			_m += (_sim->_time0 - _last) * ( tr_involts() + _v1 ) ;
			_v1 = tr_involts();
		}else{ untested();
		}
		_last = _sim->_time0;

		q_eval();
	}
};
/*--------------------------------------------------------------------------*/
double DEV_INT::tr_probe_num(const std::string&x) const{
	if (Umatch(x, "integrand")) { untested();
		return  tr_involts();
	}else if (Umatch(x, "last ")) {
		return  _last;
	}else if (Umatch(x, "avg |average")) { 
		return _m/(_sim->_time0*2.0);
	}else if (Umatch(x, "int{egral} ")) { 
		return _m;
	}
	return ELEMENT::tr_probe_num(x);
}
/*--------------------------------------------------------------------------*/
void DEV_INT::precalc_last()
{
	ELEMENT::precalc_last();
	set_constant(true);
	set_converged(true);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
DEV_INT p1;
DISPATCHER<CARD>::INSTALL
d1(&device_dispatcher, "int|integrate", &p1);
}
