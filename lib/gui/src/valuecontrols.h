#ifndef _VALUE_CONTROL__HG
#define _VALUE_CONTROL__HG

#ifdef BUILDING_CL2_DLL
# ifndef CL2_EXPORT
#   define CL2_EXPORT Q_DECL_EXPORT
# endif
#else
# ifndef CL2_EXPORT
//#   define CL2_EXPORT Q_DECL_IMPORT
#	define CL2_EXPORT 
# endif
#endif

#include <QWidget>
#include <QVariant>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>

/*! \brief Display/set PV
  *
  */
 class CL2_EXPORT ValueControl : public QWidget//, public ConstraintsInterface
 {
     Q_OBJECT
 protected:
     QString _pvName;
 public:
     ValueControl( QWidget *parent );
     virtual QVariant value() const { return QVariant(); }
	 virtual void setValue( QVariant ){}	
	 virtual float min() const { return 0; }
	 virtual float max() const { return 100; }     
	 virtual void setMin( float ){}
	 virtual void setMax( float ){}
	 QVariant dummyVar() const { return QVariant(); }
	 void ci_stdValueSet( const QVariant & v );
     virtual void attachTo( QString pvname );
     virtual ~ValueControl(){}

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
	 Q_PROPERTY( float setMin READ min WRITE setMin USER true)
	 Q_PROPERTY( float setMax READ max WRITE setMax USER true)
	 Q_PROPERTY( QVariant ci_stdValue READ dummyVar WRITE ci_stdValueSet USER true)
 };

 class CL2_EXPORT IntSliderControl : public ValueControl
 {	 
     Q_OBJECT
 private slots:
    void processSliderChange( int );
    void processSliderReleased( );
	void processSliderPressed( );
    void processSpinnerChange( int );

 private:
     QSlider *_slider;
     QSpinBox *_spinbox;
	 QLabel *_standardValue;
	 bool _ignoreChanges;
 public:
     IntSliderControl( QString pvName, QString label=QString::null, int min=0, int max=100, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );

	 virtual float min() const { return _slider->minimum(); }
	 virtual float max() const { return _slider->maximum(); }     
	 virtual void setMin( float );
	 virtual void setMax( float );

     virtual void setConstraint(QString, QVariant);

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };

 class CL2_EXPORT LineEditControl : public ValueControl
 {
     Q_OBJECT

 private slots:
    void processChange( QString );

 private:
     QLineEdit *_value;
	 QLabel *_standardValue;
	 bool _ignoreChanges;
 public:
     LineEditControl( QString pvName, QString label=QString::null, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );

     virtual void setConstraint(QString, QVariant);

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };


 class CL2_EXPORT PasswordInputControl : public ValueControl
 {
     Q_OBJECT

 private slots:
    void processChange( QString );

 private:
     QLineEdit *_value;
	 QLabel *_standardValue;
	 bool _ignoreChanges;
 public:
     PasswordInputControl( QString pvName, QString label=QString::null, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );

     virtual void setConstraint(QString, QVariant);

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };

 class CL2_EXPORT CheckBoxControl : public ValueControl
 {
     Q_OBJECT
 private slots:
    void processChecked( bool );
	void setConstraint(QString constrName, QVariant val);

 private:
     QCheckBox *_checkBox;
	 QLabel *_standardValue;

 public:
     CheckBoxControl( QString pvName, QString label=QString::null, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };

 class CL2_EXPORT TextBoxControl : public ValueControl
 {
     Q_OBJECT

 private:
     QTextEdit *_textBox;
	 
 private slots:
	 void acceptValue();

 public:
     TextBoxControl( QString pvName, QString label=QString::null, int minHeight=50, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };


 class CL2_EXPORT ComboBoxControl : public ValueControl
 {
     Q_OBJECT
 private slots:
    void processValueChange( int );

 private:
     QComboBox *_comboBox;
     int _min;
     int _max;
	 QLabel *_standardValue;

 public:
     ComboBoxControl( QString pvName, QString label=QString::null, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );
     virtual void setConstraint(QString, QVariant);

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)     
 };

 class CL2_EXPORT DblSpinBoxControl : public ValueControl
 {
     Q_OBJECT
 private slots:
    void processValueChange( double );

 private:
     QDoubleSpinBox *_spinBox;
     double _min;
     double _max;
	 QLabel *_standardValue;

 public:
     DblSpinBoxControl( double step, QString pvName, QString label=QString::null, QWidget *parent=0 );
     virtual QVariant value() const;
     virtual void setValue( QVariant );
     virtual void setConstraint(QString, QVariant);

 public:
     Q_PROPERTY( QVariant value READ value WRITE setValue USER true)
 };

#endif
