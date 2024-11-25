#include "stdafx.h"	
#include "Error.h" 

namespace Error { 
	// ñåğèè îøèáîê: 0 - 99 - ñèñòåìíûå îøèáêè
	//				100 - 109 - îøèáêè ïàğàìåòğîâ 
	//				110 - 119 - îøèáêè îòêğûòèÿ è ÷òåíèÿ
	ERROR errors[ERROR_MAX_ENTRY] = 
	{
		ERROR_ENTRY(0, "Íåäîïóñòèìûé êîä îøìèáêè"), 
		ERROR_ENTRY(1, "Ñèñòåìíûé ñáîé"),
		ERROR_ENTRY_NODEF(2),
		ERROR_ENTRY_NODEF(3),
		ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),
		ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),
		ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), 
		ERROR_ENTRY_NODEF10(20), 
		ERROR_ENTRY_NODEF10(30), 
		ERROR_ENTRY_NODEF10(40), 
		ERROR_ENTRY_NODEF10(50), 
		ERROR_ENTRY_NODEF10(60), 
		ERROR_ENTRY_NODEF10(70), 
		ERROR_ENTRY_NODEF10(80), 
		ERROR_ENTRY_NODEF10(90), 
		ERROR_ENTRY(100, "Ïàğàìåòğ -in äîëæåí áûòü çàäàí"),
		ERROR_ENTRY_NODEF(101),
		ERROR_ENTRY_NODEF(102),
		ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Ïğåâûøåíà äëèíà âõîäíîãî ïàğàìåòğà"), 
		ERROR_ENTRY_NODEF(105), 
		ERROR_ENTRY_NODEF(106), 
		ERROR_ENTRY_NODEF(107), 
		ERROR_ENTRY_NODEF(108), 
		ERROR_ENTRY_NODEF(109), 
		ERROR_ENTRY(110, "Îøèáêà ïğè îòêğûòèè ôàéëà ñ èñõîäíûì êîäîì (-in)"), 
		ERROR_ENTRY(111, "Íåäîïóñòèìûé ñèìâîë â èñõîäíîì ôàéëå (-in)"), 
		ERROR_ENTRY(112, "Îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"), 
		ERROR_ENTRY(113, "Îøèáêà ïğè ñîçäàíèè âûõîäíîãî ôàéëà (-out)"), 
		ERROR_ENTRY(114, "Îøèáêà ğàçáîğà öåïî÷êè"),
		ERROR_ENTRY(115, "Èíäåêñ ñòğîêè áîëüøå ğàçìåğà êîíòåéíåğà"), 
		ERROR_ENTRY(116, "Ôàéë íå ñóùåñòâóåò"),
		ERROR_ENTRY(117, "Îøèáêà ğàçáîğà âõîäíîé ñòğîêè"), 
		ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), 
		ERROR_ENTRY_NODEF10(130), 
		ERROR_ENTRY_NODEF10(140), 
		ERROR_ENTRY_NODEF10(150), 
		ERROR_ENTRY_NODEF10(160), 
		ERROR_ENTRY_NODEF10(170), 
		ERROR_ENTRY_NODEF10(180), 
		ERROR_ENTRY_NODEF10(190), 
		ERROR_ENTRY_NODEF100(200), 
		ERROR_ENTRY_NODEF100(300), 
		ERROR_ENTRY_NODEF100(400), 
		ERROR_ENTRY_NODEF100(500), 
		ERROR_ENTRY_NODEF100(600), 
		ERROR_ENTRY_NODEF100(700), 
		ERROR_ENTRY_NODEF100(800), 
		ERROR_ENTRY_NODEF100(900)  
	};

	ERROR geterror(int id) { 
		if (id > 0 && id < ERROR_MAX_ENTRY) { 
			return errors[id]; 
		}
		else {
			return errors[0];
		}
	}

	ERROR geterrorin(int id, int line, int col, std::string buff, std::vector<std::string> MyVector) 
	{
		if (id > 0 && id < ERROR_MAX_ENTRY) { 
			errors[id].inext.col = col; 
			errors[id].inext.line = line; 
			errors[id].inext.buff = buff; // Óñòàíîâêà áóôåğà îøèáêè
			errors[id].inext.MyVector = MyVector; // Óñòàíîâêà âåêòîğà äîïîëíèòåëüíûõ äàííûõ
			return errors[id]; 
		}
		else {
			return errors[0]; 
		}
	}

	ERROR geterrorfst(int id, int line, std::string buff) // Ôóíêöèÿ äëÿ ïîëó÷åíèÿ ïåğâîé îøèáêè ñ äîïîëíèòåëüíîé èíôîğìàöèåé
	{
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			errors[id].inext.fstline = line; 
			errors[id].inext.buff = buff; 
			return errors[id]; 
		}
		else {
			return errors[0]; 
		}
	}

} 
