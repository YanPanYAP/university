#pragma once
#define GRB_ERROR_SERIES 600
#include"Greibach.h"
#include"MFST.h"
namespace GRB {
#define NS(n) Rule::Chain::N(n)
#define	TS(n) Rule::Chain::T(n)
	Greibach greibach(NS('S'), TS('$'),
		15,
		Rule(NS('Z'), GRB_ERROR_SERIES + 1,
			1,
			Rule::Chain(5, TS('f'), TS('i'), TS('('), NS('G'), TS(')'))
		),
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			3,
			Rule::Chain(7, TS('t'), NS('Z'), TS('['), NS('N'), TS(']'), TS(';'), NS('S')),
			Rule::Chain(6, TS('t'), NS('Z'), TS('['), NS('N'), TS(']'), NS('S')),
			Rule::Chain(4, TS('m'), TS('['), NS('N'), TS(']'))
		),
		Rule(NS('G'), GRB_ERROR_SERIES + 2,
			6,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('G')),
			Rule::Chain(3, TS('i'), TS(','), NS('G')),
			Rule::Chain(3, TS('l'), TS(','), NS('G'))
		),

		Rule(NS('V'), GRB_ERROR_SERIES + 3,
			4,
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('/'))
		),
		Rule(NS('O'), GRB_ERROR_SERIES + 4,
			8,
			Rule::Chain(1, TS('|')),
			Rule::Chain(1, TS('&')),
			Rule::Chain(1, TS('!')),
			Rule::Chain(1, TS('%')),
			Rule::Chain(1, TS('>')),
			Rule::Chain(1, TS('<')),
			Rule::Chain(1, TS('}')),
			Rule::Chain(1, TS('{'))
		),
		Rule(NS('Q'), GRB_ERROR_SERIES + 5,
			2,
			Rule::Chain(3, TS('('), NS('M'), TS(')')),
			Rule::Chain(5, TS('('), NS('M'), TS(')'), NS('V'), NS('E'))
		),
		Rule(NS('L'), GRB_ERROR_SERIES + 6,
			5,
			Rule::Chain(2, TS('R'), NS('Q')),
			Rule::Chain(2, TS('S'), NS('Q')),
			Rule::Chain(2, TS('U'), NS('Q')),
			Rule::Chain(2, TS('A'), NS('Q')),
			Rule::Chain(2, TS('C'), NS('Q'))
		),

		Rule(NS('D'), GRB_ERROR_SERIES + 7,
			1,
			Rule::Chain(2, TS('t'), NS('W'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 8,
			8,
			Rule::Chain(3, TS('i'), TS('='), NS('M')),
			Rule::Chain(3, TS('i'), TS('='), NS('E')),
			Rule::Chain(3, TS('i'), TS('='), NS('L')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), NS('V'), NS('E')),
			Rule::Chain(4, TS('i'), TS('='), TS('-'), NS('M')),
			Rule::Chain(4, TS('i'), TS('='), TS('-'), NS('E')),
			Rule::Chain(4, TS('i'), TS('='), TS('-'), NS('L')),
			Rule::Chain(6, TS('i'), TS('='), TS('-'), NS('E'), NS('V'), NS('E'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 9,
			12,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), TS(';')),
			Rule::Chain(2, TS('l'), TS(';')),
			Rule::Chain(3, TS('i'), NS('V'), NS('E')),
			Rule::Chain(3, TS('l'), NS('V'), NS('E')),
			Rule::Chain(3, TS('i'), NS('O'), NS('E')),
			Rule::Chain(3, TS('l'), NS('O'), NS('E')),
			Rule::Chain(2, TS('S'), NS('Q')),
			Rule::Chain(2, TS('U'), NS('Q')),
			Rule::Chain(2, TS('R'), NS('Q')),
			Rule::Chain(4, TS('i'), TS('('), NS('G'), TS(')'))
		),

		Rule(NS('M'), GRB_ERROR_SERIES + 10,
			2,
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i'))

		),

		Rule(NS('K'), GRB_ERROR_SERIES + 11,
			3,
			Rule::Chain(5, TS(':'), NS('E'), TS('$'), NS('A'), TS('$')),
			Rule::Chain(4, TS(':'), NS('E'), TS('$'), TS('$')),
			Rule::Chain(5, TS(':'), NS('E'), TS('$'), TS('$'), NS('N'))
		),
		Rule(NS('A'), GRB_ERROR_SERIES + 12,
			3,
			Rule::Chain(3, TS('c'), TS(':'), NS('Y')),
			Rule::Chain(3, TS('w'), TS(':'), NS('Y')),
			Rule::Chain(2, TS('v'), NS('Y'))
		),
		Rule(NS('Y'), GRB_ERROR_SERIES + 13,
			2,
			Rule::Chain(4, TS('['), NS('N'), TS(']'), NS('A')),
			Rule::Chain(3, TS('['), NS('N'), TS(']'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 14,
			14,
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('n'), NS('D'), TS(';'), NS('N')),
			Rule::Chain(3, TS('n'), NS('D'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('d'), NS('E'), TS(';')),
			Rule::Chain(4, TS('d'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('?'), NS('K'), NS('N')),
			Rule::Chain(2, TS('?'), NS('K')),
			Rule::Chain(3, TS('r'), NS('M'), TS(';')),
			Rule::Chain(4, NS('N'), TS(';'), TS('r'), NS('M'))
		)
	);

}