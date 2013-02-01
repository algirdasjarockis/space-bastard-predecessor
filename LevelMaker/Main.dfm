object Form1: TForm1
  Left = 561
  Top = 310
  Width = 1119
  Height = 687
  Caption = 'SpaceBastard'#39's LevelMaker'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 617
    Width = 1104
    Height = 20
    Panels = <
      item
        Width = 550
      end
      item
        Width = 100
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object StringGrid1: TStringGrid
    Left = 0
    Top = 0
    Width = 865
    Height = 617
    ColCount = 14
    DefaultColWidth = 60
    DefaultRowHeight = 60
    DefaultDrawing = False
    FixedCols = 0
    RowCount = 10
    FixedRows = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnDrawCell = StringGrid1DrawCell
    OnMouseUp = StringGrid1MouseUp
  end
  object cbEnemies: TComboBox
    Left = 871
    Top = 3
    Width = 153
    Height = 21
    ItemHeight = 13
    TabOrder = 2
    Text = 'cbEnemies'
    OnChange = cbEnemiesChange
  end
  object cbPowerups: TComboBox
    Left = 871
    Top = 27
    Width = 153
    Height = 21
    ItemHeight = 13
    TabOrder = 3
    Text = 'cbPowerups'
    OnChange = cbPowerupsChange
  end
  object gbEnemy: TGroupBox
    Left = 871
    Top = 56
    Width = 233
    Height = 241
    Caption = 'Enemy'
    Enabled = False
    TabOrder = 4
    object Label2: TLabel
      Left = 24
      Top = 31
      Width = 34
      Height = 13
      Caption = 'Health:'
    end
    object Label3: TLabel
      Left = 24
      Top = 55
      Width = 37
      Height = 13
      Caption = 'Weight:'
    end
    object Label4: TLabel
      Left = 24
      Top = 79
      Width = 44
      Height = 13
      Caption = 'Speed X:'
    end
    object Label5: TLabel
      Left = 28
      Top = 126
      Width = 38
      Height = 13
      Caption = 'Moving:'
    end
    object lAmmo: TLabel
      Left = 24
      Top = 175
      Width = 38
      Height = 13
      Caption = 'Ammo:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 24
      Top = 103
      Width = 44
      Height = 13
      Caption = 'Speed Y:'
    end
    object Label7: TLabel
      Left = 24
      Top = 151
      Width = 43
      Height = 13
      Caption = 'Min. diff.:'
    end
    object eHealth: TEdit
      Left = 72
      Top = 24
      Width = 145
      Height = 21
      TabOrder = 0
    end
    object eWeight: TEdit
      Left = 72
      Top = 48
      Width = 145
      Height = 21
      TabOrder = 1
    end
    object eSpeedX: TEdit
      Left = 72
      Top = 72
      Width = 145
      Height = 21
      TabOrder = 2
    end
    object eMoving: TEdit
      Left = 72
      Top = 119
      Width = 145
      Height = 21
      TabOrder = 3
    end
    object btnEnemySave: TButton
      Left = 72
      Top = 205
      Width = 145
      Height = 25
      Caption = 'Save'
      TabOrder = 4
      OnClick = btnEnemySaveClick
    end
    object eSpeedY: TEdit
      Left = 72
      Top = 96
      Width = 145
      Height = 21
      TabOrder = 5
    end
    object eMinDiffEnemy: TEdit
      Left = 72
      Top = 144
      Width = 145
      Height = 21
      TabOrder = 6
    end
  end
  object btnDown: TButton
    Left = 871
    Top = 576
    Width = 233
    Height = 41
    Caption = 'Apatinis'
    TabOrder = 5
    OnClick = btnDownClick
  end
  object btnUp: TButton
    Left = 873
    Top = 528
    Width = 231
    Height = 41
    Caption = 'Virsutinis'
    TabOrder = 6
    OnClick = btnUpClick
  end
  object btnLoad: TButton
    Left = 1031
    Top = 0
    Width = 73
    Height = 49
    Caption = 'Load'
    TabOrder = 7
    OnClick = btnLoadClick
  end
  object gbPowerup: TGroupBox
    Left = 871
    Top = 304
    Width = 233
    Height = 153
    Caption = 'Powerup'
    Enabled = False
    TabOrder = 8
    object Label8: TLabel
      Left = 28
      Top = 50
      Width = 34
      Height = 13
      Caption = 'Speed:'
    end
    object Label9: TLabel
      Left = 24
      Top = 74
      Width = 43
      Height = 13
      Caption = 'Min. diff.:'
    end
    object lType: TLabel
      Left = 24
      Top = 97
      Width = 33
      Height = 13
      Caption = 'Type:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label11: TLabel
      Left = 33
      Top = 27
      Width = 30
      Height = 13
      Caption = 'Value:'
    end
    object eSpeed: TEdit
      Left = 72
      Top = 44
      Width = 145
      Height = 21
      TabOrder = 0
    end
    object eMinDiffPwup: TEdit
      Left = 72
      Top = 68
      Width = 145
      Height = 21
      TabOrder = 1
    end
    object btnPowerupSave: TButton
      Left = 72
      Top = 120
      Width = 145
      Height = 25
      Caption = 'Save'
      TabOrder = 2
      OnClick = btnPowerupSaveClick
    end
    object eValue: TEdit
      Left = 72
      Top = 20
      Width = 145
      Height = 21
      TabOrder = 3
    end
  end
  object CColorGrid1: TCColorGrid
    Left = 876
    Top = 464
    Width = 224
    Height = 48
    GridOrdering = go8x2
    BackgroundEnabled = False
    TabOrder = 9
    OnClick = CColorGrid1Click
  end
  object MainMenu1: TMainMenu
    Left = 808
    Top = 568
    object File1: TMenuItem
      Caption = 'File'
      object Open1: TMenuItem
        Caption = 'Open..'
        ShortCut = 16463
        OnClick = Open1Click
      end
      object Save1: TMenuItem
        Caption = 'Save..'
        ShortCut = 16467
        OnClick = Save1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object ExitandplayD1: TMenuItem
        Caption = 'Exit and play :D'
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'XML files (*.xml)|*.xml|Any file (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 712
    Top = 568
  end
  object SaveDialog1: TSaveDialog
    Filter = 'XML files (*.xml)|*.xml|Any file (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 744
    Top = 568
  end
end
