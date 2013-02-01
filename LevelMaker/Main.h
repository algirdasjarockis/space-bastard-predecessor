//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "funcs.h"
#include "Level.h"
#include <Menus.hpp>
#include "CGRID.h"
#include <Dialogs.hpp>
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *StatusBar1;
    TStringGrid *StringGrid1;
    TComboBox *cbEnemies;
    TComboBox *cbPowerups;
    TGroupBox *gbEnemy;
    TEdit *eHealth;
    TEdit *eWeight;
    TEdit *eSpeedX;
    TEdit *eMoving;
    TButton *btnDown;
    TButton *btnUp;
    TButton *btnLoad;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TButton *btnEnemySave;
    TGroupBox *gbPowerup;
    TLabel *lAmmo;
    TEdit *eSpeedY;
    TLabel *Label6;
    TEdit *eMinDiffEnemy;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *eSpeed;
    TLabel *Label9;
    TEdit *eMinDiffPwup;
    TButton *btnPowerupSave;
    TLabel *lType;
    TLabel *Label11;
    TEdit *eValue;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TCColorGrid *CColorGrid1;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TMenuItem *Open1;
    TMenuItem *Save1;
    TMenuItem *N1;
    TMenuItem *ExitandplayD1;
    void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnLoadClick(TObject *Sender);
    void __fastcall cbEnemiesChange(TObject *Sender);
    void __fastcall StringGrid1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnEnemySaveClick(TObject *Sender);
    void __fastcall CColorGrid1Click(TObject *Sender);
    void __fastcall cbPowerupsChange(TObject *Sender);
    void __fastcall btnPowerupSaveClick(TObject *Sender);
    void __fastcall btnUpClick(TObject *Sender);
    void __fastcall btnDownClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Save1Click(TObject *Sender);
    void __fastcall Open1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);

        unsigned int currentPage;       // dabartinis squad'as/puslapis

        Enemies * enemies;              // enemy listas is xml'o su basic info
        Powerups * powerups;            // powerup listas is xml'o

        Enemy *currentEnemy;            // siuo metu pasirinktas enemy is listo
        Powerup *currentPowerup;        // siuo metu pasirinktas pwup is listo
        LevelEntity * currentEntity;    // siuo metu pasirinktas map'o objektas

        Level level;                    // pagrindinis masyvas - pasaulio matrica :))

        TColor currentColor;            // esama spalva objektu pavadinimams

        // metodai
        void showCurrentEntityAttrs();
        void displaySquad();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
