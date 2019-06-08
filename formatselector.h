#ifndef FORMATSELECTOR_H_INCLUDED
#define FORMATSELECTOR_H_INCLUDED

#include <QComboBox>

class FormatSelector
      : public QComboBox
{
public:
	enum {
		HEX, DEC
	};
public:
	FormatSelector();
};

#endif // FORMATSELECTOR_H_INCLUDED
