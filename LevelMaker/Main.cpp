//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGRID"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    currentEnemy = NULL;
    currentEntity = NULL;
    currentPage = 0;
    currentColor = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    if( State.Contains(gdFixed) ) {
    }
    else if( State.Contains(gdSelected) ) {
        StringGrid1->Canvas->Brush->Color = static_cast<TColor>(RGB(255, 205, 155));
        StringGrid1->Canvas->Font->Style = TFontStyles() >> fsBold;
        StringGrid1->Canvas->Font->Color = currentColor;
        StringGrid1->Canvas->FillRect(Rect);
    }
    else {
        StringGrid1->Canvas->Brush->Color = clWhite;
        StringGrid1->Canvas->Font->Color = currentColor;
        StringGrid1->Canvas->FillRect(Rect);
    }

    AnsiString text = StringGrid1->Cells[ACol][ARow];
    StringGrid1->Canvas->TextRect(Rect, Rect.Left, Rect.Top, text);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnLoadClick(TObject *Sender)
{
    if(enemies)
        enemies->clear();
    enemies = loadEnemies("..\\SpaceBastard\\Debug\\base\\enemies.xml");
    if(enemies) {
        for(unsigned int i = 0; i < enemies->size(); i++) {
            cbEnemies->Items->Add( AnsiString((*enemies)[i].name.c_str()) );
        }
        cbEnemies->ItemIndex = 0;
        gbEnemy->Enabled = true;
    }

    if(powerups)
        powerups->clear();
    powerups = loadPowerups("..\\SpaceBastard\\Debug\\base\\powerups.xml");
    if(powerups) {
        for(unsigned int i = 0; i < powerups->size(); i++) {
            cbPowerups->Items->Add( AnsiString((*powerups)[i].name.c_str()) );
        }
        cbPowerups->ItemIndex = 0;
        gbPowerup->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cbEnemiesChange(TObject *Sender)
{
    Enemy e = (*enemies)[cbEnemies->ItemIndex];

    if(currentEnemy) {
        delete currentEnemy;
    }
    if(currentPowerup) {
        delete currentPowerup;
        currentPowerup = NULL;
    }    
    currentEnemy = new Enemy(e);
    currentEntity = NULL;

    showCurrentEntityAttrs();

    gbEnemy->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cbPowerupsChange(TObject *Sender)
{
    Powerup p = (*powerups)[cbPowerups->ItemIndex];

    if(currentPowerup) {
        delete currentPowerup;
    }
    if(currentEnemy) {
        delete currentEnemy;
        currentEnemy = NULL;
    }
    currentPowerup = new Powerup(p);
    currentEntity = NULL;

    showCurrentEntityAttrs();

    gbPowerup->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int col, row;
    StringGrid1->MouseToCell(X, Y, col, row);

    // PAZYMEJIMAS
    if(Shift.Contains(ssCtrl)) {
        if(level[currentPage][col][row]) {
            currentEnemy = NULL;
            currentPowerup = NULL;
            currentEntity = level[currentPage][col][row];
            showCurrentEntityAttrs();
        }
        return;
    }

    // TRYNIMAS
    if(Button == mbRight) {
        if(level[currentPage][col][row]) {
            if(currentEntity == level[currentPage][col][row])
                currentEntity = NULL;
            delete level[currentPage][col][row];
            level[currentPage][col][row] = NULL;
            StringGrid1->Cells[col][row] = "";
        }
        return;
    }

    // PRIDEJIMAS
    if(currentEntity) {
        // jei pasirinktas objektas is mapo

        if(level[currentPage][col][row])
            delete level[currentPage][col][row];

        level[currentPage][col][row] = new LevelEntity(*currentEntity);
        if(currentEntity->type == enemy)
            StringGrid1->Cells[col][row] = currentEntity->enemyInfo.name.c_str();
        else StringGrid1->Cells[col][row] = currentEntity->powerupInfo.name.c_str();
    }
    else if(currentEnemy) {
        // jei pasirinktas objektas is listo

        if(level[currentPage][col][row])
            delete level[currentPage][col][row];

        StringGrid1->Cells[col][row] = currentEnemy->name.c_str();
        LevelEntity *ent = new LevelEntity();
        ent->enemyInfo = *currentEnemy;
        ent->type = enemy;
        ent->xspeed = ent->yspeed = currentEnemy->speed;
        ent->minDiff = 1;
        level[currentPage][col][row] = ent;
    }
    else if(currentPowerup) {
        // jei pasirinktas objektas is listo

        if(level[currentPage][col][row])
            delete level[currentPage][col][row];

        StringGrid1->Cells[col][row] = currentPowerup->name.c_str();
        LevelEntity *ent = new LevelEntity();
        ent->powerupInfo = *currentPowerup;
        ent->type = powerup;
        ent->minDiff = 1;
        level[currentPage][col][row] = ent;
    }
}
//---------------------------------------------------------------------------

void TForm1::showCurrentEntityAttrs()
{
    if(currentEntity) {
        if(currentEntity->type == enemy) {
            eHealth->Text = IntToStr(currentEntity->enemyInfo.health);
            eWeight->Text = IntToStr(currentEntity->enemyInfo.weight);
            eSpeedX->Text = FloatToStr(currentEntity->xspeed);
            eSpeedY->Text = FloatToStr(currentEntity->yspeed);
            eMoving->Text = AnsiString(currentEntity->enemyInfo.moving.c_str());
            eMinDiffEnemy->Text = IntToStr(currentEntity->minDiff);
            lAmmo->Caption = AnsiString("Ammo: ") + AnsiString(currentEntity->enemyInfo.ammo.c_str());
        }
        else {
            eValue->Text = currentEntity->powerupInfo.value.c_str();
            eSpeed->Text = FloatToStr(currentEntity->powerupInfo.speed);
            eMinDiffPwup->Text = IntToStr(currentEntity->minDiff);
            lType->Caption = AnsiString("Type: ") + AnsiString(currentEntity->powerupInfo.type.c_str());
        }
    }
    else if(currentEnemy) {
        eHealth->Text = IntToStr(currentEnemy->health);
        eWeight->Text = IntToStr(currentEnemy->weight);
        eSpeedX->Text = FloatToStr(currentEnemy->speed);
        eSpeedY->Text = FloatToStr(currentEnemy->speed);
        eMoving->Text = AnsiString(currentEnemy->moving.c_str());
        eMinDiffEnemy->Text = "1";
        lAmmo->Caption = AnsiString("Ammo: ") + AnsiString(currentEnemy->ammo.c_str());
    }
    else if(currentPowerup) {
        eSpeed->Text = FloatToStr(currentPowerup->speed);
        eValue->Text = AnsiString(currentPowerup->value.c_str());
        eMinDiffPwup->Text = "1";
        lType->Caption = AnsiString("Type: ") + AnsiString(currentPowerup->type.c_str());
    }
}

void __fastcall TForm1::btnEnemySaveClick(TObject *Sender)
{
    if(currentEntity) {
        if(currentEntity->type == enemy) {
            currentEntity->enemyInfo.health = StrToInt(eHealth->Text);
            currentEntity->enemyInfo.weight = StrToInt(eWeight->Text);
            currentEntity->xspeed = StrToFloat(eSpeedX->Text);
            currentEntity->yspeed = StrToFloat(eSpeedY->Text);
            currentEntity->enemyInfo.moving = string(eMoving->Text.c_str());
            currentEntity->minDiff = StrToInt(eMinDiffEnemy->Text);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnPowerupSaveClick(TObject *Sender)
{
    if(currentEntity) {
        if(currentEntity->type == powerup) {
            currentEntity->powerupInfo.value = eValue->Text.c_str();
            currentEntity->powerupInfo.speed = StrToFloat(eSpeed->Text);
            currentEntity->minDiff = StrToInt(eMinDiffPwup->Text);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CColorGrid1Click(TObject *Sender)
{
    currentColor = CColorGrid1->ForegroundColor;
}
//---------------------------------------------------------------------------

void TForm1::displaySquad()
{
    //for(Level::iterator it = level.begin(); it != level.end(); ++it) {
    for(int i = 0; i < 14; i++) {
        for(int j = 0; j < 10; j++) {
            StringGrid1->Cells[i][j] = "";
            if(level[currentPage][i][j]) {
                if(level[currentPage][i][j]->type == enemy)
                    StringGrid1->Cells[i][j] = level[currentPage][i][j]->enemyInfo.name.c_str();
                else StringGrid1->Cells[i][j] = level[currentPage][i][j]->powerupInfo.name.c_str();
            }
        }
    }
}
void __fastcall TForm1::btnUpClick(TObject *Sender)
{
    currentPage++;
    displaySquad();
    StatusBar1->Panels->Items[1]->Text = "Squad: " + IntToStr(currentPage);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnDownClick(TObject *Sender)
{
    if(currentPage > 0)
        currentPage--;
    displaySquad();
    StatusBar1->Panels->Items[1]->Text = "Squad: " + IntToStr(currentPage);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
    StatusBar1->Panels->Items[0]->Text = "Ready..";
    StatusBar1->Panels->Items[1]->Text = "Squad: " + IntToStr(currentPage);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Save1Click(TObject *Sender)
{
    if(SaveDialog1->Execute()) {
        saveToFile(SaveDialog1->FileName.c_str(), level);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
    if(OpenDialog1->Execute()) {
        level.clear();
        level = readFromFile(OpenDialog1->FileName.c_str());
        currentPage = 0;
        displaySquad();
    }
}
//---------------------------------------------------------------------------

