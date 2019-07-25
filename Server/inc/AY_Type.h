/* ------------------------------------------------------------------------- */
/* Generic/Portable Type & Macro header () --------------- */
/* ------------------------------------------------------------------------- */
#ifndef	_AY_TYPE_H_
#define	_AY_TYPE_H_
/* ------------------------------------------------------------------------- */
//#include <DK_mcu.h>
/* ------------------------------------------------------------------------- */
#undef		True
#undef		TRUE
#define		True	1
#define		TRUE	1
/* ------------------------------------------------------------------------- */
#undef		False
#undef		FALSE
#define		False	0
#define		FALSE	0
/* ------------------------------------------------------------------------- */
#define		DT_CHR	'C'
#define		DT_STR	'S'
#define		DT_BIN	'B'
#define		DT_INT2	'W'
#define		DT_INT4	'I'
#define		DT_INT8	'M'
#define		DT_FLT4	'R'
#define		DT_FLT8	'F'
#define		DT_DATE	'D'
#define		DT_UNIQ	'P'
#define		DT_ALL	"CSBWIMRFDP"
/* ------------------------------------------------------------------------- */
#define		_szPTR	sayzof(void *)
/* ------------------------------------------------------------------------- */
#ifdef		_MSC_VER
#define		__FONK__	__FUNCDNAME__
#else
#define		__FONK__	__PRETTY_FUNCTION__
#endif
/* ------------------------------------------------------------------------- */
#define		NUL		0x00	///< NULl
#define		SOH		0x01	///< Start Of Header
#define		STX		0x02	///< Start of TeXt
#define		ETX		0x03	///< End of TeXt
#define		EOT		0x04	///< End Of Transmission
#define		ENQ		0x05	///< ENQuiry
#define		ACK		0x06	///< ACKnowledge
#define		BEL		0x07	///< BELl
#define		B_S		0x08	///< BackSpace
#define		TAB		0x09	///< TAB
#define		L_F		0x0A	///< Line Feed
#define		V_T		0x0B	///< Vertical Tab
#define		F_F		0x0C	///< Form Feed
#define		C_R		0x0D	///< Carriage Return
#define		S_O		0x0E	///< Shift Out
#define		S_I		0x0F	///< Shift In
#define		DLE		0x10	///< Data Link Escape
#define		DC1		0x11	///< Device Control 1
#define		DC2		0x12	///< Device Control 2
#define		DC3		0x13	///< Device Control 3
#define		DC4		0x14	///< Device Control 4
#define		NAK		0x15	///< Not AcKnowledged
#define		SYN		0x16	///< SYNchronous idle
#define		ETB		0x17	///< End of Transmission Block
#define		CAN		0x18	///< CANcel
#define		E_M		0x19	///< End of Medium
#define		SUB		0x1A	///< SUBstitute
#define		ESC		0x1B	///< ESCape
#define		F_S		0x1C	///< Field Separator
#define		G_S		0x1D	///< Group Separator
#define		R_S		0x1E	///< Record Separator
#define		U_S		0x1F	///< Unit Separator
/* ------------------------------------------------------------------------- */
#define		_10u0	1L
#define		_10u1	10L
#define		_10u2	100L
#define		_10u3	1000L
#define		_10u4	10000L
#define		_10u5	100000L
#define		_10u6	1000000L
#define		_10u7	10000000L
#define		_10u8	100000000L
#define		_10u9	1000000000L
/* ------------------------------------------------------------------------- */
#define		_10u10	10000000000LL
#define		_10u11	100000000000LL
#define		_10u12	1000000000000LL
#define		_10u13	10000000000000LL
#define		_10u14	100000000000000LL
#define		_10u15	1000000000000000LL
#define		_10u16	10000000000000000LL
#define		_10u17	100000000000000000LL
#define		_10u18	1000000000000000000LL
#define		_10u19	10000000000000000000LL
/* ------------------------------------------------------------------------- */
#define		_1K				_K(1)
#define		_2K				_K(2)
#define		_4K				_K(4)
#define		_8K				_K(8)
#define		_16K			_K(16)
#define		_32K			_K(32)
#define		_64K			_K(64)
#define		_128K			_K(128)
#define		_256K			_K(256)
#define		_512K			_K(512)

#define		_K(_c)			(((_c)) << 10)
#define		_M(_c)			(((_c)) << 20)
#define		_G(_c)			(((_c)) << 30)
/* ------------------------------------------------------------------------- */
#define		NUM_TOODD(_v)	(((_v) - 1) | +1)
#define		NUM_ISODD(_v)	(((_v) & 1) == 1)
#define		NUM_ISPW2(_v)	(((_v) & ((_v) - 1)) && (_v))
#define		NUM_ISEVN(_v)	(((_v) & 1) == 0)
#define		NUM_TOEVN(_v)	(((_v) + 1) & -2)
#define		NUM_TONAN(_v)	do{if(sizeof(_v) == sizeof(Flt4)){Ui32 _i4 = 0x7FF80000UL; MemCpy(&(_v), &_i4, sizeof(Flt4));} if(sizeof(_v) == sizeof(Flt8)){Ui64 _i8 = 0x7FF8000000000000ULL; MemCpy(&(_v), &_i8, sizeof(Flt8));}}while(0);
/* ------------------------------------------------------------------------- */
#undef		_MAX
#define		_MAX(_a, _b)	(((_a) > (_b)) ? (_a) : (_b))
#undef		_MIN
#define		_MIN(_a, _b)	(((_a) < (_b)) ? (_a) : (_b))
/* ------------------------------------------------------------------------- */
#define		TAGGED_SIZEOF(_o)		int SZ_##_o = sayzof(_o); SZ_##_o += 1; SZ_##_o -= 1;
#define		ABSTRACT_SIZE(_s, _o)	int      _s = sayzof(_o);      _s += 1;      _s -= 1;
/* ------------------------------------------------------------------------- */
#define		STATIC_ASSERW(_c, _m)		typedef char STATIC_ASSERT_##_m[(2 * (!!(_c))) - 1];
#define		STATIC_ASSERV(_c, _m, _l)	STATIC_ASSERW(_c, _m##_AT_LINE_##_l)
#define		STATIC_ASSERU(_c, _m, _l)	STATIC_ASSERV(_c, _m, _l)
#define		STATIC_ASSERT(_c, _m)		STATIC_ASSERU(_c, _m, __LINE__)
/* ------------------------------------------------------------------------- */
#define		SERI_0004(_v)	SERI_0001((_v)+   0),	SERI_0001((_v)+   1),	SERI_0001((_v)+   2),	SERI_0001((_v)+   3)
#define		SERI_0016(_v)	SERI_0004((_v)+   0),	SERI_0004((_v)+   4),	SERI_0004((_v)+   8),	SERI_0004((_v)+  12)
#define		SERI_0064(_v)	SERI_0016((_v)+   0),	SERI_0016((_v)+  16),	SERI_0016((_v)+  32),	SERI_0016((_v)+  48)
#define		SERI_0256(_v)	SERI_0064((_v)+   0),	SERI_0064((_v)+  64),	SERI_0064((_v)+ 128),	SERI_0064((_v)+ 192)
#define		SERI_1024(_v)	SERI_0256((_v)+   0),	SERI_0256((_v)+ 256),	SERI_0256((_v)+ 512),	SERI_0256((_v)+ 768)
#define		SERI_4096(_v)	SERI_1024((_v)+   0),	SERI_1024((_v)+1024),	SERI_1024((_v)+2048),	SERI_1024((_v)+3072)
/* ------------------------------------------------------------------------- */
#define		sayzof(_v)			(((int)sizeof(_v)))
#define		 bitof(_v)			(((int)sizeof(_v) / 1) * 8)
#define		 lenof(_v)			(((int)sizeof(_v) / 1) - 1)
#define		wlenof(_v)			(((int)sizeof(_v) / 2) - 1)
#define		 cntof(_a)			( (int)(sizeof(_a) / sizeof((_a)[0])))
#define		 maxof(_a)			( (int) cntof(_a) - 1)
#define		 ofset(_s, _m)		(((int)(&(((_s *)NULL)->_m))))
#define		 taban(_s, _m, _p)	((void *)((size_t)(_p) - (size_t)(ofset(_s, _m))))
#define		 endof(_s, _m)		(((int)(&(((_s *)NULL)->_m))) + sayzof(((_s *)NULL)->_m))
#define		 idxof(_b, _p)		((int)((((size_t)(_p)) - ((size_t)(_b))) / sizeof((_b)[0])))
#define		difint(_l, _r)		(((int)(((size_t)(_l)) - ((size_t)(_r)))))
#define		difabs(_l, _r)		((((size_t)(_l)) < ((size_t)(_r))) ? (((size_t)(_r)) - ((size_t)(_l))) : (((size_t)(_l)) - ((size_t)(_r))))
/* ------------------------------------------------------------------------- */
#define		ISNUM(_c)			(('0' <= (_c)) && ((_c) <= '9'))
#define		ISLWR(_c)			(('a' <= (_c)) && ((_c) <= 'z'))
#define		ISUPR(_c)			(('A' <= (_c)) && ((_c) <= 'Z'))
#define		ISCHR(_c)			(ISLWR(_c) || ISUPR(_c))
#define		ISALF(_c)			(ISNUM(_c) || ISCHR(_c))
#define		ISHEX_LWR(_c)		(ISNUM(_c) || (('a' <= (_c)) && ((_c) <= 'f')))
#define		ISHEX_UPR(_c)		(ISNUM(_c) || (('A' <= (_c)) && ((_c) <= 'F')))
#define		ISHEX(_c)			(ISNUM(_c) || (('a' <= (_c)) && ((_c) <= 'f')) || (('A' <= (_c)) && ((_c) <= 'F')))
/* ------------------------------------------------------------------------- */
#define		TOLWR(_c)			((char)((ISUPR(_c)) ? ((_c) - 'A' + 'a') : (_c)))
#define		DOLWR(_c)			do{if(ISUPR(_c)){(_c) -= 'A'; (_c) += 'a';}}while(0);
#define		TOUPR(_c)			((char)((ISLWR(_c)) ? ((_c) - 'a' + 'A') : (_c)))
#define		DOUPR(_c)			do{if(ISLWR(_c)){(_c) -= 'a'; (_c) += 'A';}}while(0);
#define		EQCHR(_l, _r)		(((_l) == (_r)) || (((_l) + ('A' - 'a')) == (_r)) || ((_l) == ((_r) + ('A' - 'a'))))
/* ------------------------------------------------------------------------- */
#define		BELOW(_v, _u)		do{if((_u) < (_v)){(_v) = (_u);}}while(0);
#define		ABOVE(_l, _v)		do{if((_v) < (_l)){(_v) = (_l);}}while(0);
#define		AMONG(_l, _v, _u)	do{ABOVE(_l, _v); BELOW(_v, _u);}while(0);
/* ------------------------------------------------------------------------- */
#define		RND_LWR(_v, _i)		((((_v)) / (_i)) * (_i))
#define		RND_UPR(_v, _i)		((((_v) + (_i) - 1) / (_i)) * (_i))
#define		RND_MAX(_v, _i)		((((_v) + (_i) + (_i) - 1) / (_i)) * (_i))
#define		RND_BIT(_v, _b)		((((_v) + (+1LL << (_b))) - 1LL) & (-1LL << (_b)))
/* ------------------------------------------------------------------------- */
#define		FLP_MSK(_v, _m)		do{(_v) = ((_v) ^ ( (_m)));}while(0);
#define		MSK_CLR(_v, _m)		(((_v) & (_m)) == 0x00)
#define		SET_MSK(_v, _m)		do{(_v) = ((_v) | ( (_m)));}while(0);
#define		MSK_SET(_v, _m)		(((_v) & (_m)) == (_m))
#define		CLR_MSK(_v, _m)		do{(_v) = ((_v) & (~(_m)));}while(0);
/* ------------------------------------------------------------------------- */
#define		FLP_BIT(_v, _b)		FLP_MSK((_v), (1ULL << (_b)))
#define		BIT_CLR(_v, _b)		MSK_CLR((_v), (1ULL << (_b)))
#define		SET_BIT(_v, _b)		SET_MSK((_v), (1ULL << (_b)))
#define		BIT_SET(_v, _b)		MSK_SET((_v), (1ULL << (_b)))
#define		CLR_BIT(_v, _b)		CLR_MSK((_v), (1ULL << (_b)))
#define		BIT_VAL(_v, _b)		(((_v) >> (_b)) & 0x01)
/* ------------------------------------------------------------------------- */
#define		BiA_CLR(_a, _b)		BIT_CLR(((_a)[(_b) / bitof((_a)[0])]), ((_b) % bitof((_a)[0])))
#define		SET_BiA(_a, _b)		SET_BIT(((_a)[(_b) / bitof((_a)[0])]), ((_b) % bitof((_a)[0])))
#define		BiA_SET(_a, _b)		BIT_SET(((_a)[(_b) / bitof((_a)[0])]), ((_b) % bitof((_a)[0])))
#define		CLR_BiA(_a, _b)		CLR_BIT(((_a)[(_b) / bitof((_a)[0])]), ((_b) % bitof((_a)[0])))
#define		BiA_VAL(_a, _b)		BIT_VAL(((_a)[(_b) / bitof((_a)[0])]), ((_b) % bitof((_a)[0])))
/* ------------------------------------------------------------------------- */
#define		IX_NBBL(_v, _x)		((Bayt)(((_v) >> ((_x) << 2)) & 0x0000000FLL))
#define		LO_NBBL(_v)			IX_NBBL(_v, 0)
#define		HI_NBBL(_v)			IX_NBBL(_v, 1)
#define		IX_BAYT(_v, _x)		((Bayt)(((_v) >> ((_x) << 3)) & 0x000000FFLL))
#define		LO_BAYT(_v)			IX_BAYT(_v, 0)
#define		HI_BAYT(_v)			IX_BAYT(_v, 1)
#define		IX_WORD(_v, _x)		((Ui16)(((_v) >> ((_x) << 4)) & 0x0000FFFFLL))
#define		LO_WORD(_v)			IX_WORD(_v, 0)
#define		HI_WORD(_v)			IX_WORD(_v, 1)
#define		IX_LONG(_v, _x)		((Ui32)(((_v) >> ((_x) << 5)) & 0xFFFFFFFFLL))
#define		LO_LONG(_v)			IX_LONG(_v, 0)
#define		HI_LONG(_v)			IX_LONG(_v, 1)
/* ------------------------------------------------------------------------- */
#define		SET_IXN(_v, _n, _i)	do{(_v) &= ~(0x0000000FLL << ((_i) << 2)); (_v) |= (((_n) & 0x0000000FLL) << ((_i) << 2));}while(0);
#define		SET_LON(_v, _n)		SET_IXN(_v, _n, 0)
#define		SET_HIN(_v, _n)		SET_IXN(_v, _n, 1)
#define		SET_IXB(_v, _n, _i)	do{(_v) &= ~(0x000000FFLL << ((_i) << 3)); (_v) |= (((_n) & 0x000000FFLL) << ((_i) << 3));}while(0);
#define		SET_LOB(_v, _n)		SET_IXN(_v, _n, 0)
#define		SET_HIB(_v, _n)		SET_IXN(_v, _n, 1)
#define		SET_IXW(_v, _n, _i)	do{(_v) &= ~(0x0000FFFFLL << ((_i) << 4)); (_v) |= (((_n) & 0x0000FFFFLL) << ((_i) << 4));}while(0);
#define		SET_LOW(_v, _n)		SET_IXN(_v, _n, 0)
#define		SET_HIW(_v, _n)		SET_IXN(_v, _n, 1)
#define		SET_IXL(_v, _n, _i)	do{(_v) &= ~(0xFFFFFFFFLL << ((_i) << 5)); (_v) |= (((_n) & 0xFFFFFFFFLL) << ((_i) << 5));}while(0);
#define		SET_LOL(_v, _n)		SET_IXL(_v, _n, 0)
#define		SET_HIL(_v, _n)		SET_IXL(_v, _n, 1)
/* ------------------------------------------------------------------------- */
#define		CPY_IXN(_v, _n, _i)	SET_IXN(_v, (((_n) >> ((_i) << 2)) & 0x0000000FLL), _i)
#define		CPY_LON(_v, _n)		CPY_IXN(_v, _n, 0)
#define		CPY_HIN(_v, _n)		CPY_IXN(_v, _n, 1)
#define		CPY_IXB(_v, _n, _i)	SET_IXB(_v, (((_n) >> ((_i) << 3)) & 0x000000FFLL), _i)
#define		CPY_LOB(_v, _n)		CPY_IXB(_v, _n, 0)
#define		CPY_HIB(_v, _n)		CPY_IXB(_v, _n, 1)
#define		CPY_IXW(_v, _n, _i)	SET_IXW(_v, (((_n) >> ((_i) << 4)) & 0x0000FFFFLL), _i)
#define		CPY_LOW(_v, _n)		CPY_IXW(_v, _n, 0)
#define		CPY_HIW(_v, _n)		CPY_IXW(_v, _n, 1)
#define		CPY_IXL(_v, _n, _i)	SET_IXL(_v, (((_n) >> ((_i) << 5)) & 0xFFFFFFFFLL), _i)
#define		CPY_LOL(_v, _n)		CPY_IXL(_v, _n, 0)
#define		CPY_HIL(_v, _n)		CPY_IXL(_v, _n, 1)
/* ------------------------------------------------------------------------- */
#define		DIGIT_0(_v)		(((_v) / _10u0) % 10)
#define		DIGIT_1(_v)		(((_v) / _10u1) % 10)
#define		DIGIT_2(_v)		(((_v) / _10u2) % 10)
#define		DIGIT_3(_v)		(((_v) / _10u3) % 10)
#define		DIGIT_4(_v)		(((_v) / _10u4) % 10)
#define		DIGIT_5(_v)		(((_v) / _10u5) % 10)
#define		DIGIT_6(_v)		(((_v) / _10u6) % 10)
#define		DIGIT_7(_v)		(((_v) / _10u7) % 10)
#define		DIGIT_8(_v)		(((_v) / _10u8) % 10)
#define		DIGIT_9(_v)		(((_v) / _10u9) % 10)
/* ------------------------------------------------------------------------- */
#define		BCD_to_I2D(_v)	(((((_v) /   16) & 15) *   10) + ((((_v) /   1) & 15) *   1))
#define		I2D_to_BCD(_v)	(((((_v) /   10) % 10) *   16) | ((((_v) /   1) % 10) *   1))
#define		BCD_to_I3D(_v)	(((((_v) /  256) & 15) *  100) + ((((_v) /  16) & 15) *  10) + ((((_v) /  1) & 15) *  1))
#define		I3D_to_BCD(_v)	(((((_v) /  100) % 10) *  256) | ((((_v) /  10) % 10) *  16) | ((((_v) /  1) % 10) *  1))
#define		BCD_to_I4D(_v)	(((((_v) / 4096) & 15) * 1000) + ((((_v) / 256) & 15) * 100) + ((((_v) / 16) & 15) * 10) + ((((_v) / 1) & 15) * 1))
#define		I4D_to_BCD(_v)	(((((_v) / 1000) % 10) * 4096) | ((((_v) / 100) % 10) * 256) | ((((_v) / 10) % 10) * 16) | ((((_v) / 1) % 10) * 1))
/* ------------------------------------------------------------------------- */
#define		GET_VAL(_bv, _hv, _bh, _bl)	do{(_bv) = (((_hv) & (((1LL << ((_bh) + 1)) - 1LL) ^ ((1LL << (_bl)) - 1))) >> (_bl));}while(0);
#define		SET_VAL(_hv, _bv, _bh, _bl)	do{(_hv) &= ~(((1LL << ((_bh) + 1)) - 1LL) ^ ((1LL << (_bl)) - 1LL)); (_hv) |= (((_bv) & ((1LL << ((_bh) - (_bl) + 1)) - 1LL)) << (_bl));}while(0);
/* ------------------------------------------------------------------------- */
#define		ENDIAN_REVERT(_v)			do{int __i; bayt *__v = (bayt *)&(_v); bayt __b[sizeof(_v)]; if((2 <= sizeof(_v)) && (sizeof(_v) <= 16)){MCPY(__b, (_v)); for(__i = 0; __i <= lenof(_v); __i++){__v[lenof(_v) - __i] = __b[__i];}}}while(0);

#define		XEP_to_TIP(_i, _p, _e, _t)	do{	int __i; _t __t = 0x00; bayt *__s = (bayt *)(_p);										\
											if((_e) > 0){for(__i = 0; __i <= lenof(_t); __i++){(__t) <<= 8; (__t) |= __s[__i];}}	\
											if((_e) < 1){for(__i = lenof(_t); __i >= 0; __i--){(__t) <<= 8; (__t) |= __s[__i];}}	\
											(_i) = __t; /* For In-Place Endian Operation ! */										\
										}while(0);

#define		TIP_to_XEP(_p, _i, _e, _t)	do{	int __i; _t __t = (_i); bayt *__d = (bayt *)(_p);												\
											if((_e) > 0){for(__i = lenof(_t); __i >= 0; __i--){__d[__i] = (bayt)(__t & 0xFF); __t >>= 8;}}	\
											if((_e) < 1){for(__i = 0; __i <= lenof(_t); __i++){__d[__i] = (bayt)(__t & 0xFF); __t >>= 8;}}	\
											/* This is already In-Place Endian Operation ! */												\
										}while(0);

#define		XEP_to_VAL(_i, _p, _e, _t)	do{	int __i; bayt __s[sizeof(_t)]; MemCpy(__s, (void *)(_p), sizeof(_t)); ( _i) = 0x00;		\
											if((_e) > 0){for(__i = 0; __i <= lenof(_t); __i++){( _i) <<= 8; ( _i) |= __s[__i];}}	\
											if((_e) < 1){for(__i = lenof(_t); __i >= 0; __i--){( _i) <<= 8; ( _i) |= __s[__i];}}	\
											/* Note that (_i) and (_t) macro parameters may be of different datatype(s) !?!? */		\
										}while(0);

#define		VAL_to_XEP(_p, _i, _e, _t)	do{_t __x = (_t)(_i); TIP_to_XEP(_p, __x, _e, _t);}while(0);
/* ------------------------------------------------------------------------- */
#define		LEP_to_S16(_i, _p)			XEP_to_TIP(_i, _p, 0, Si16)
#define		LEP_to_U16(_i, _p)			XEP_to_TIP(_i, _p, 0, Ui16)
#define		LEP_to_S32(_i, _p)			XEP_to_TIP(_i, _p, 0, Si32)
#define		LEP_to_U32(_i, _p)			XEP_to_TIP(_i, _p, 0, Ui32)
#define		LEP_to_S64(_i, _p)			XEP_to_TIP(_i, _p, 0, Si64)
#define		LEP_to_U64(_i, _p)			XEP_to_TIP(_i, _p, 0, Ui64)
// ...
#define		BEP_to_S16(_i, _p)			XEP_to_TIP(_i, _p, 1, Si16)
#define		BEP_to_U16(_i, _p)			XEP_to_TIP(_i, _p, 1, Ui16)
#define		BEP_to_S32(_i, _p)			XEP_to_TIP(_i, _p, 1, Si32)
#define		BEP_to_U32(_i, _p)			XEP_to_TIP(_i, _p, 1, Ui32)
#define		BEP_to_S64(_i, _p)			XEP_to_TIP(_i, _p, 1, Si64)
#define		BEP_to_U64(_i, _p)			XEP_to_TIP(_i, _p, 1, Ui64)

#define		S16_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Si16)
#define		U16_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Ui16)
#define		S32_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Si32)
#define		U32_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Ui32)
#define		S64_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Si64)
#define		U64_to_LEP(_p, _i)			TIP_to_XEP(_p, _i, 0, Ui64)
// ...
#define		S16_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Si16)
#define		U16_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Ui16)
#define		S32_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Si32)
#define		U32_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Ui32)
#define		S64_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Si64)
#define		U64_to_BEP(_p, _i)			TIP_to_XEP(_p, _i, 1, Ui64)
/* ------------------------------------------------------------------------- */
#define		LEP_to_I16(_i, _p)			XEP_to_VAL(_i, _p, 0, Ui16)
#define		LEP_to_I32(_i, _p)			XEP_to_VAL(_i, _p, 0, Ui32)
#define		LEP_to_I64(_i, _p)			XEP_to_VAL(_i, _p, 0, Ui64)
// ...
#define		BEP_to_I16(_i, _p)			XEP_to_VAL(_i, _p, 1, Ui16)
#define		BEP_to_I32(_i, _p)			XEP_to_VAL(_i, _p, 1, Ui32)
#define		BEP_to_I64(_i, _p)			XEP_to_VAL(_i, _p, 1, Ui64)

#define		I16_to_LEP(_p, _i)			VAL_to_XEP(_p, _i, 0, Ui16)
#define		I32_to_LEP(_p, _i)			VAL_to_XEP(_p, _i, 0, Ui32)
#define		I64_to_LEP(_p, _i)			VAL_to_XEP(_p, _i, 0, Ui64)
// ...
#define		I16_to_BEP(_p, _i)			VAL_to_XEP(_p, _i, 1, Ui16)
#define		I32_to_BEP(_p, _i)			VAL_to_XEP(_p, _i, 1, Ui32)
#define		I64_to_BEP(_p, _i)			VAL_to_XEP(_p, _i, 1, Ui64)
/* ------------------------------------------------------------------------- */
#ifdef		MEMORY_BUG
extern int   MEMORY_BUG_line;
extern char *MEMORY_BUG_file;
extern char *MEMORY_BUG_func;
#endif		/* MEMORY_BUG */
/* ------------------------------------------------------------------------- */
#ifndef		MEMORY_BUG
#define		StrLen(_d)			strlen( (char *)(_d))
#define		StrUpr(_d)			strupr( (char *)(_d))
#define		StrLwr(_d)			strlwr( (char *)(_d))
#define		StrCpy(_d, _s)		strcpy( (char *)(_d), (char *)(_s))
#define		StrCmp(_d, _s)		strcmp( (char *)(_d), (char *)(_s))
#define		StrXmp(_d, _s)		stricmp((char *)(_d), (char *)(_s))
#define		StrCat(_d, _s)		strcat( (char *)(_d), (char *)(_s))
#define		StrChr(_s, _c)		strchr( (char *)(_s), (int   )(_c))
#define		StrStr(_s, _x)		strstr( (char *)(_s), (char *)(_x))
#define		StrNpy(_d, _s, _c)	strncpy((char *)(_d), (char *)(_s), (size_t)(_c))
#define		StrXpy(_d, _s)		(((char *)(_d))[sizeof(_d) - 1] = 0, strncpy((char *)(_d), (char *)(_s), (size_t)(sizeof(_d) - 1)))
#define		MemSet(_d, _s, _c)	memset( (_d), (_s), (size_t)(_c))
#define		MemCpy(_d, _s, _c)	memcpy( (_d), (_s), (size_t)(_c))
#define		MemCmp(_d, _s, _c)	memcmp( (_d), (_s), (size_t)(_c))
#define		MemChr(_d, _x, _c)	memchr( (_d), (_x), (size_t)(_c))
#define		MemMov(_d, _s, _c)	memmove((_d), (_s), (size_t)(_c))
#else		/* MEMORY_BUG */
#define		StrLen(_d)			(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strlen( (char *)(_d)))
#define		StrUpr(_d)			(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strupr( (char *)(_d)))
#define		StrLwr(_d)			(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strlwr( (char *)(_d)))
#define		StrCpy(_d, _s)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strcpy( (char *)(_d), (char *)(_s)))
#define		StrCmp(_d, _s)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strcmp( (char *)(_d), (char *)(_s)))
#define		StrXmp(_d, _s)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, stricmp((char *)(_d), (char *)(_s)))
#define		StrCat(_d, _s)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strcat( (char *)(_d), (char *)(_s)))
#define		StrChr(_s, _c)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strchr( (char *)(_s), (int   )(_c)))
#define		StrStr(_s, _x)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strstr( (char *)(_s), (char *)(_x)))
#define		StrNpy(_d, _s, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, strncpy((char *)(_d), (char *)(_s), (size_t)(_c)))
#define		StrXpy(_d, _s)		(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, ((char *)(_d))[sizeof(_d) - 1] = 0, strncpy((char *)(_d), (char *)(_s), (size_t)(sizeof(_d) - 1)))
#define		MemSet(_d, _s, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, memset( (_d), (_s), (size_t)(_c)))
#define		MemCpy(_d, _s, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, memcpy( (_d), (_s), (size_t)(_c)))
#define		MemCmp(_d, _s, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, memcmp( (_d), (_s), (size_t)(_c)))
#define		MemChr(_d, _x, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, memchr( (_d), (_x), (size_t)(_c)))
#define		MemMov(_d, _s, _c)	(MEMORY_BUG_func = __FONK__, MEMORY_BUG_file = __FILE__, MEMORY_BUG_line = __LINE__, memmove((_d), (_s), (size_t)(_c)))
#endif		/* !MEMORY_BUG */
/* ------------------------------------------------------------------------- */
#define		 ZERO(_o)			MemSet(&(_o), 0x00, sizeof( (_o)))
#define		xZERO(_p)			MemSet( (_p), 0x00, sizeof(*(_p)))
#define		uZERO(_p, _s)		MemSet( (_p), 0x00,         (_s) )
#define		mZERO(_a, _m)		do{int _i; for(_i = 0; _i < cntof(_a); _i++){ZERO((_a)[_i]._m);}}while(0);
#define		gZERO(_x, _s)		do{int _i; volatile char * _p = (volatile char *)(_x); for(_i = 0; _i < (int)(_s); _i++){_p[_i] = 0x00;}}while(0);
#define		sZERO(_o)			gZERO(&(_o), sizeof(_o))
/* ------------------------------------------------------------------------- */
#define		 MSET(_o, _v)		MemSet(&(_o), (_v), sizeof( (_o)))
#define		xMSET(_p, _v)		MemSet( (_p), (_v), sizeof(*(_p)))
#define		uMSET(_p, _v, _s)	MemSet( (_p), (_v),         (_s) )
#define		mMSET(_a, _m, _v)	do{int _i; for(_i = 0; _i < cntof(_a); _i++){MSET((_a)[_i]._m, (_v));}}while(0);
/* ------------------------------------------------------------------------- */
#define		 SXMP(_d, _s)		(StrXmp( (_d),  (_s)               ) == 0)
#define		 SCMP(_d, _s)		(StrCmp( (_d),  (_s)               ) == 0)
#define		 MCMP(_d, _s, _l)	(MemCmp( (_d),  (_s),         (_l) ) == 0)
#define		sMCMP(_d, _s)		(MemCmp( (_d),  (_s),  lenof( (_s))) == 0)
#define		lMCMP(_d, _s)		(MemCmp(&(_d), &(_s), sizeof( (_d))) == 0)
#define		rMCMP(_d, _s)		(MemCmp(&(_d), &(_s), sizeof( (_s))) == 0)
/* ------------------------------------------------------------------------- */
#define		 MCPY(_d, _s)		MemCpy(&(_d), &(_s), sizeof( (_d)))
#define		xMCPY(_d, _s)		MemCpy( (_d), &(_s), sizeof(*(_d)))
#define		MCPYx(_d, _s)		MemCpy(&(_d),  (_s), sizeof( (_d)))
#define		sMCPY(_d, _s)		do{size_t _c = sizeof(_d); if(_c > sizeof(_s)){char *_p = (char *)&(_d); _c = sizeof(_s); uZERO(&_p[_c], (sizeof(_d) - sizeof(_s)));} MemCpy(&(_d), &(_s), _c);}while(0);
/* ------------------------------------------------------------------------- */
#define		x_JOB(_j)				do{X_MEM_barrier(); do{_j;}while(0); X_MEM_barrier();}while(0);
#define		m_VAL(_s)				(X_GET_barrier(),              (_s))
#define		v_VAL(_t, _s)			(X_GET_barrier(), (volatile _t)(_s))
#define		m_LET(_d, _i)			do{(_d) = (_i); X_SET_barrier();}while(0);
#define		v_LET(_t, _d, _i)		do{                 volatile _t *__d = (volatile _t *)(&(_d));                                                             *__d = (_i); X_SET_barrier();}while(0);
#define		v_SET(_t, _d, _s)		do{                 volatile _t *__d = (volatile _t *)(&(_d)); volatile _t *__s = (volatile _t *)(&(_s));                  *__d = *__s; X_SET_barrier();}while(0);
#define		x_SET(_t, _d, _s)		do{X_MEM_barrier(); volatile _t *__d = (volatile _t *)(&(_d)); volatile _t *__s = (volatile _t *)(&(_s));                  *__d = *__s; X_SET_barrier();}while(0);
#define		v_GET(_t, _d, _s)		do{                 volatile _t *__d = (volatile _t *)(&(_d)); volatile _t *__s = (volatile _t *)(&(_s)); X_GET_barrier(); *__d = *__s;                 }while(0);
/* ------------------------------------------------------------------------- */
#define		iAEXX(_a, _x, _p)				iAEXZ(_a,    cntof(_a), _x, _p)
#define		iAEXZ(_a, _c, _x, _p)			iAEXR(_a, 0,        _c, _x, _p)
#define		iAEXR(_a, _h, _t, _x, _p)		do{size_t _i; for(_i = (size_t)(_h); _i < (size_t)(_t); _i++){(_p) = &((_a)[_i]); do{_x;}while(0);}}while(0);
#define		oAIDX(_x, _a, _p)				do{(_x) = -1; (_x) = (int)(((Si64)(((size_t)(_p)) - ((size_t)(&((_a)[0]))))) / ((Si64)(sizeof((_a)[0]))));}while(0);
/* ------------------------------------------------------------------------- */
#define		vASET(_a, _v)					do{size_t _i; for(_i = 0; _i < cntof(_a); _i++){(_a)[_i]    = (_v);}}while(0);
#define		vAIDX(_x, _a, _v)				vAIDZ(_x, _a,    cntof(_a), _v)
#define		vAIDZ(_x, _a, _c, _v)			vAIDR(_x, _a, 0,        _c, _v)
#define		vAIDR(_x, _a, _h, _t, _v)		do{size_t _i; (_x) = -1; for(_i = (size_t)(_h); _i < (size_t)(_t); _i++){if((_a)[_i]    == (_v)){(_x) = (int)_i; _i = (size_t)(_t);}}}while(0);
/* ------------------------------------------------------------------------- */
#define		mASET(_a, _m, _v)				do{size_t _i; for(_i = 0; _i < cntof(_a); _i++){(_a)[_i]._m = (_v);}}while(0);
#define		mAIDX(_x, _a, _m, _v)			mAIDZ(_x, _a,    cntof(_a), _m, _v)
#define		mAIDZ(_x, _a, _c, _m, _v)		mAIDR(_x, _a, 0,        _c, _m, _v)
#define		mAIDR(_x, _a, _h, _t, _m, _v)	do{size_t _i; (_x) = -1; for(_i = (size_t)(_h); _i < (size_t)(_t); _i++){if((_a)[_i]._m == (_v)){(_x) = (int)_i; _i = (size_t)(_t);}}}while(0);
/* ------------------------------------------------------------------------- */
#define		vSIDX(_x, _a, _v)				vSIDZ(_x, _a,    cntof(_a), _v)
#define		vSIDZ(_x, _a, _c, _v)			vSIDR(_x, _a, 0,        _c, _v)
#define		vSIDR(_x, _a, _h, _t, _v)		do{	int _mid, _min = (int)(_h), _max = (int)(_t) - 1; (_x) = -1;			\
												while(_min <= _max)														\
												{																		\
													_mid = (_min + _max) / 2;											\
													if((_a)[_mid]    == (_v)){_x = _mid; break;}						\
													if((_a)[_mid]    <  (_v)){_min = _mid + 1;}else{_max = _mid - 1;}	\
												}																		\
											}while(0);
#define		mSIDX(_x, _a, _m, _v)			mSIDZ(_x, _a,    cntof(_a), _m, _v)
#define		mSIDZ(_x, _a, _c, _m, _v)		mSIDR(_x, _a, 0,        _c, _m, _v)
#define		mSIDR(_x, _a, _h, _t, _m, _v)	do{	int _mid, _min = (int)(_h), _max = (int)(_t) - 1; (_x) = -1;			\
												while(_min <= _max)														\
												{																		\
													_mid = (_min + _max) / 2;											\
													if((_a)[_mid]._m == (_v)){_x = _mid; break;}						\
													if((_a)[_mid]._m <  (_v)){_min = _mid + 1;}else{_max = _mid - 1;}	\
												}																		\
											}while(0);
/* ------------------------------------------------------------------------- */
#define	MSC_PRAGMA(_p)	// MSC #pragma expansion macro
#define	GCC_PRAGMA(_p)	// GCC #pragma expansion macro
#define	ARM_PRAGMA(_p)	// ARM #pragma expansion macro

#define	CEIC_0()		// Conditional Expression Is Constant = 0
#define	CEIC_1()		// Conditional Expression Is Constant = 1
/* ------------------------------------------------------------------------- */
#ifdef		__GNUC__

#ifndef	__OPTIMIZE__
#define	DK_MCU_BUX	1
#endif	/* __OPTIMIZE__ */

#ifndef	__CHAR_UNSIGNED__
#include <ChkCompilerOptions.h>
#include <CharMustBeUnsigned.h>
#endif	/* __CHAR_UNSIGNED__ */

#ifdef	__ARMEB__
#define	DK_CPU_BE
#undef	DK_CPU_LE
#else
#undef	DK_CPU_BE
#define	DK_CPU_LE
#endif

#define	DK_PACK_L	/* NONE */
#define	DK_PACK_R	__attribute__ ((__packed__))

typedef		signed char				si08, Si08;
typedef		unsigned char			ui08, Ui08;
typedef		signed short			si16, Si16;
typedef		unsigned short			ui16, Ui16;
typedef		signed int				si32, Si32;
typedef		unsigned int			ui32, Ui32;
typedef		signed long long int	si64, Si64;
typedef		unsigned long long int	ui64, Ui64;

#undef	_DK_WIND_
#undef	_DK_UNIX_
#define	_DK_UNIX_	1

#undef	GCC_PRAGMA
#define	GCC_PRAGMA(_p)	_Pragma(_p)

#endif		/* __GNUC__ */
/* ------------------------------------------------------------------------- */
#ifdef		_MSC_VER

#ifdef		MEMORY_BUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// ...
#define	DEBUG_CLIENTBLOCK	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define	new					DEBUG_CLIENTBLOCK

#endif		/* MEMORY_BUG */

#ifdef	_DEBUG
#define	DK_MCU_BUX	1
#endif	/* _DEBUG */

//#ifndef	_CHAR_UNSIGNED
//#include <ChkCompilerOptions.h>
//#include <CharMustBeUnsigned.h>
//#endif	/* _CHAR_UNSIGNED */

#undef	WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN

#pragma warning(disable:4002)
#pragma warning(disable:4091)
#pragma warning(disable:4127)
#pragma warning(disable:4200)
#pragma warning(disable:4201)
#pragma warning(disable:4211)
#pragma warning(disable:4214)
#pragma warning(disable:4505)
#pragma warning(disable:4514)
#pragma warning(disable:4668)
#pragma warning(disable:4710)
#pragma warning(disable:4739)
#pragma warning(disable:4820)
#pragma warning(disable:4986)
#pragma warning(disable:4987)
#pragma warning(disable:4996)
// ...
#pragma execution_character_set("utf-8")

#ifndef	snprintf
#define	snprintf	_snprintf
#endif	/* snprintf */

#ifdef	_M_MRX000
#define	DK_CPU_BE
#undef	DK_CPU_LE
#else
#undef	DK_CPU_BE
#define	DK_CPU_LE
#endif

#define	DK_PACK_L	/* NONE */
#define	DK_PACK_R	__declspec(align(1))	/* This is WRONG */

typedef		signed __int8			si08, Si08;
typedef		unsigned __int8			ui08, Ui08;
typedef		signed __int16			si16, Si16;
typedef		unsigned __int16		ui16, Ui16;
typedef		signed __int32			si32, Si32;
typedef		unsigned __int32		ui32, Ui32;
typedef		signed __int64			si64, Si64;
typedef		unsigned __int64		ui64, Ui64;

#undef	_DK_WIND_
#undef	_DK_UNIX_
#define	_DK_WIND_	1

#undef	MSC_PRAGMA
#define	MSC_PRAGMA(_p)	__pragma(_p)

#undef	CEIC_0
#define	CEIC_0()	__pragma(warning(push)); __pragma(warning(disable:4127));
#undef	CEIC_1
#define	CEIC_1()	__pragma(warning(pop));

#endif		/* _MSC_VER */
/* ------------------------------------------------------------------------- */
#ifdef		__BORLANDC__

#ifdef	_DEBUG
#define	DK_MCU_BUX	1
#endif	/* _DEBUG */

#ifndef	_CHAR_UNSIGNED
#include <ChkCompilerOptions.h>
#include <CharMustBeUnsigned.h>
#endif	/* _CHAR_UNSIGNED */

#undef	WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN

#undef	DK_CPU_BE
#define	DK_CPU_LE

#define	DK_PACK_L	/* NONE */
#define	DK_PACK_R	__declspec(align(1))

typedef		signed __int8			si08, Si08;
typedef		unsigned __int8			ui08, Ui08;
typedef		signed __int16			si16, Si16;
typedef		unsigned __int16		ui16, Ui16;
typedef		signed __int32			si32, Si32;
typedef		unsigned __int32		ui32, Ui32;
typedef		signed __int64			si64, Si64;
typedef		unsigned __int64		ui64, Ui64;

#define	_MSC_VER		9
#define	__debugbreak()	do{}while(0);

#undef	_DK_WIND_
#undef	_DK_UNIX_
#define	_DK_WIND_	1

#endif		/* __BORLANDC__ */
/* ------------------------------------------------------------------------- */
#ifdef		__CC_ARM

#if		(__OPTIMISE_LEVEL < 1)
#define	DK_MCU_BUX	1
#endif	/* (__OPTIMISE_LEVEL < 1) */

#ifdef	__FEATURE_SIGNED_CHAR
#include <ChkCompilerOptions.h>
#include <CharMustBeUnsigned.h>
#endif	/* __FEATURE_SIGNED_CHAR */

#ifndef	stricmp
#define	stricmp	strcasecmp
#endif	/* stricmp */

#pragma	anon_unions			/* This enables Anonymous Union(s)	 */
#pragma diag_suppress 55	/* This suppresses macro error  55-D */
#pragma diag_suppress 225	/* This suppresses macro error 225-D */

#ifdef	__BIG_ENDIAN
#define	DK_CPU_BE
#undef	DK_CPU_LE
#else
#undef	DK_CPU_BE
#define	DK_CPU_LE
#endif

#define DK_PACK_L	__packed
#define DK_PACK_R	/* NONE */

typedef		signed char				si08, Si08;
typedef		unsigned char			ui08, Ui08;
typedef		signed short			si16, Si16;
typedef		unsigned short			ui16, Ui16;
typedef		signed int				si32, Si32;
typedef		unsigned int			ui32, Ui32;
typedef		signed long long int	si64, Si64;
typedef		unsigned long long int	ui64, Ui64;

#undef	_DK_WIND_
#undef	_DK_UNIX_

#undef	ARM_PRAGMA
#define	ARM_PRAGMA(_p)	_Pragma(_p)

#endif		/* __CC_ARM */
/* ------------------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>

typedef		intptr_t		SS_t;		// ssize_t type...
typedef		size_t			US_t;		// size_t type...
typedef		unsigned char	Bool;		// boolean type...
typedef		float			flt4, Flt4; // 32-bit float...
typedef		double			flt8, Flt8; // 64-bit float...
typedef		signed char		harf, Harf; // surely signed...
typedef		unsigned char	bayt, Bayt; // surely unsigned...
/* ------------------------------------------------------------------------- */
#pragma pack(push, 2)

typedef struct _S_dt{

	Ui16	YYYY;
	Bayt	MO;
	Bayt	DD;
	Bayt	HH;
	Bayt	MI;
	Bayt	SS;
	Bayt	XX;

}S_dt; // DateTime

#pragma pack(pop)
/* ------------------------------------------------------------------------- */
#pragma pack(push, 2)

typedef union _Mx16{
	Si16	S;
	Ui16	U;
	Bayt	B[sizeof(Ui16)];
}Mx16;

#pragma pack(pop)

#pragma pack(push, 4)

typedef union _Mx32{
	Si32	S;
	Ui32	U;
	Bayt	B[sizeof(Ui32)];
}Mx32;

#pragma pack(pop)

#pragma pack(push, 8)

typedef union _Mx64{
	Si64	S;
	Ui64	U;
	Bayt	B[sizeof(Ui64)];
}Mx64;

#pragma pack(pop)
/* ------------------------------------------------------------------------- */
#ifndef	DK_MCU_BUG
#ifndef	DK_MCU_BUX
#define	DK_MCU_BUG	0
#else
#define	DK_MCU_BUG	1
#endif	/* DK_MCU_BUX */
#endif	/* DK_MCU_BUG */

#ifndef	DK_MCU_CHK
#define	DK_MCU_CHK	0
#endif	/* DK_MCU_CHK */
/* ------------------------------------------------------------------------- */
#ifdef	DK_CPU_LE

#define	LE_I16(_v)	(_v)
#define	LE_I32(_v)	(_v)
#define	BE_I16(_v)	(((((_v) >>  0) & 0xFF) <<  8) | ((((_v) >>  8) & 0xFF) <<  0))
#define	BE_I32(_v)	(((((_v) >>  0) & 0xFF) << 24) | ((((_v) >>  8) & 0xFF) << 16) | ((((_v) >> 16) & 0xFF) <<  8) | ((((_v) >> 24) & 0xFF) <<  0))

#define	PTR_to_I16(_i, _p)	LEP_to_I16(_i, _p)
#define	I16_to_PTR(_p, _i)	I16_to_LEP(_p, _i)
#define	PTR_to_I32(_i, _p)	LEP_to_I32(_i, _p)
#define	I32_to_PTR(_p, _i)	I32_to_LEP(_p, _i)
#define	PTR_to_I64(_i, _p)	LEP_to_I64(_i, _p)
#define	I64_to_PTR(_p, _i)	I64_to_LEP(_p, _i)

#endif	/* DK_CPU_LE */

#ifdef	DK_CPU_BE

#define	BE_I16(_v)	(_v)
#define	BE_I32(_v)	(_v)
#define	LE_I16(_v)	(((((_v) >>  0) & 0xFF) <<  8) | ((((_v) >>  8) & 0xFF) <<  0))
#define	LE_I32(_v)	(((((_v) >>  0) & 0xFF) << 24) | ((((_v) >>  8) & 0xFF) << 16) | ((((_v) >> 16) & 0xFF) <<  8) | ((((_v) >> 24) & 0xFF) <<  0))

#define	PTR_to_I16(_i, _p)	BEP_to_I16(_i, _p)
#define	I16_to_PTR(_p, _i)	I16_to_BEP(_p, _i)
#define	PTR_to_I32(_i, _p)	BEP_to_I32(_i, _p)
#define	I32_to_PTR(_p, _i)	I32_to_BEP(_p, _i)
#define	PTR_to_I64(_i, _p)	BEP_to_I64(_i, _p)
#define	I64_to_PTR(_p, _i)	I64_to_BEP(_p, _i)

#endif	/* DK_CPU_BE */
/* ------------------------------------------------------------------------- */
#ifdef		__CC_ARM

#if		DK_MCU == DK_MCU_LPC2368

#if		__OPTIMISE_LEVEL == 0
#define	DK_wait_DIVIDER	298LL
#elif	__OPTIMISE_LEVEL == 1
#define	DK_wait_DIVIDER	208LL
#elif	__OPTIMISE_LEVEL == 2
#define	DK_wait_DIVIDER	208LL
#elif	__OPTIMISE_LEVEL == 3
#define	DK_wait_DIVIDER	208LL
#endif

#define		t_NOP			14		/* NOP time == _10u9 / 72000000 */

#define		DK_wait_050N()		do{__nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_100N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_500N()		do{DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); __nop();}while(0);

#elif	DK_MCU == DK_MCU_LPC1768

#if		__OPTIMISE_LEVEL == 0
#define	DK_wait_DIVIDER	218LL
#elif	__OPTIMISE_LEVEL == 1
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 2
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 3
#define	DK_wait_DIVIDER	159LL
#endif

#define		t_NOP			8		/* NOP time == _10u9 / 125000000 */

#define		DK_wait_050N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_100N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_500N()		do{DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N();  __nop(); __nop(); __nop();}while(0);

#elif	DK_MCU == DK_MCU_LPC1788

#if		__OPTIMISE_LEVEL == 0
#define	DK_wait_DIVIDER	218LL
#elif	__OPTIMISE_LEVEL == 1
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 2
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 3
#define	DK_wait_DIVIDER	159LL
#endif

#define		t_NOP			8		/* NOP time == _10u9 / 125000000 */

#define		DK_wait_050N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_100N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_500N()		do{DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N();  __nop(); __nop(); __nop();}while(0);

#elif	DK_MCU == DK_MCU_LPC1549

#if		__OPTIMISE_LEVEL == 0
#define	DK_wait_DIVIDER	218LL
#elif	__OPTIMISE_LEVEL == 1
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 2
#define	DK_wait_DIVIDER	159LL
#elif	__OPTIMISE_LEVEL == 3
#define	DK_wait_DIVIDER	159LL
#endif

#define		t_NOP			8		/* NOP time == _10u9 / 125000000 */

#define		DK_wait_050N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_100N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define		DK_wait_500N()		do{DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N();  __nop(); __nop(); __nop();}while(0);

#else

#include <YouMustSetup_DK_MCU.h>

#endif	/* DK_MCU == DK_MCU_LPC1768 */

#define	DK_quot_300N(_f)		(((_f) + DK_wait_DIVIDER - 1LL) / DK_wait_DIVIDER)
#define	DK_wait_300N(_n)		do{register si32 _c = DK_quot_300N(_n); do{__schedule_barrier(); DK_wait_100N(); __schedule_barrier(); _c--;}while(_c > 0);}while(0);

#else

#define	__irq
#define	__nop()
#define	__current_pc() 0
#define	DK_wait_DIVIDER	1
#define	__schedule_barrier()

#define	DK_quot_300N(_f)	(((_f) + DK_wait_DIVIDER - 1LL) / DK_wait_DIVIDER)
#define	DK_wait_050N()		do{__nop(); __nop(); __nop(); __nop();}while(0);
#define	DK_wait_100N()		do{__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}while(0);
#define	DK_wait_500N()		do{DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); DK_wait_100N(); __nop();}while(0);
#define	DK_wait_300N(_n)	do{register si32 _c = DK_quot_300N(_n); do{__schedule_barrier(); DK_wait_100N(); __schedule_barrier(); _c--;}while(_c > 0);}while(0);

#endif		/* __CC_ARM */
/* ------------------------------------------------------------------------- */
#define	DK_MAC_OUI	"\x00\x00\x36"				/* ATARI (de-funct computer) */
#define	DK_USB_MAC	"\x00\x00\x36\x44\x4B\x4E"	/* ATARI (de-funct computer) */
#define	DK_MDM_MAC	"\x00\x00\x36\x44\x4B\x4D"	/* ATARI (de-funct computer) */
#define	DK_IP4_CLN	"\x0A\x44\x4B\x09"			/* SubNetwork : 10.'D'.'K'.9 */
#define	DK_IP4_SRV	"\x0A\x44\x4B\x01"			/* SubNetwork : 10.'D'.'K'.1 */
#define	DK_MBUS_V0	"DKV0MBUS"					/* RainbowID for  ModBus::V0 */
#define	DK_GWAY_V0	"DK00GWAY"					/* RainbowID for GateWay::V0 */
/* ------------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/* ------------------------------------------------------------------------- */
#endif /* _AY_TYPE_H_ */
