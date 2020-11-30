object formMain: TformMain
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'fOS Application ['#1055#1088#1080#1083#1086#1078#1077#1085' '#1082#1086#1084#1091#1085#1080#1082#1072#1094#1080#1086#1085#1077#1085' '#1087#1088#1086#1090#1086#1082#1086#1083']'
  ClientHeight = 401
  ClientWidth = 563
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 120
  TextHeight = 16
  object gboxTask1: TGroupBox
    Left = 0
    Top = 60
    Width = 563
    Height = 80
    Align = alTop
    Caption = ' Task 1 '
    TabOrder = 1
    object pbarTask1: TProgressBar
      Left = 2
      Top = 57
      Width = 559
      Height = 21
      Align = alBottom
      TabOrder = 0
    end
    object buttonSuspend1: TButton
      Left = 485
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Suspend'
      TabOrder = 2
      OnClick = buttonSuspend1Click
    end
    object buttonResume1: TButton
      Left = 485
      Top = 33
      Width = 75
      Height = 25
      Caption = 'Resume'
      TabOrder = 3
      OnClick = buttonResume1Click
    end
    object stTask1: TStaticText
      Left = 13
      Top = 35
      Width = 227
      Height = 20
      AutoSize = False
      BevelInner = bvNone
      BevelKind = bkFlat
      TabOrder = 4
    end
    object buttonStop1: TButton
      Left = 411
      Top = 9
      Width = 75
      Height = 49
      Caption = 'STOP'
      TabOrder = 1
      OnClick = buttonStop1Click
    end
  end
  object gboxTask2: TGroupBox
    Left = 0
    Top = 140
    Width = 563
    Height = 80
    Align = alTop
    Caption = ' Task 2 '
    TabOrder = 2
    object pbarTask2: TProgressBar
      Left = 2
      Top = 57
      Width = 559
      Height = 21
      Align = alBottom
      TabOrder = 0
    end
    object stTask2: TStaticText
      Left = 13
      Top = 35
      Width = 227
      Height = 20
      AutoSize = False
      BevelInner = bvNone
      BevelKind = bkFlat
      TabOrder = 1
    end
    object buttonSuspend2: TButton
      Left = 485
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Suspend'
      TabOrder = 3
      OnClick = buttonSuspend1Click
    end
    object buttonResume2: TButton
      Left = 485
      Top = 33
      Width = 75
      Height = 25
      Caption = 'Resume'
      TabOrder = 4
      OnClick = buttonResume1Click
    end
    object buttonStop2: TButton
      Left = 411
      Top = 9
      Width = 75
      Height = 49
      Caption = 'STOP'
      TabOrder = 2
      OnClick = buttonStop1Click
    end
  end
  object buttonStart: TButton
    Left = 0
    Top = 321
    Width = 563
    Height = 80
    Align = alClient
    Caption = #1057'   '#1058'   '#1040'   '#1056'   '#1058
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = buttonStartClick
  end
  object gboxSysStatus: TGroupBox
    Left = 0
    Top = 0
    Width = 563
    Height = 60
    Align = alTop
    Caption = ' System Status '
    TabOrder = 3
    object labelSysTimer: TLabel
      Left = 13
      Top = 24
      Width = 54
      Height = 16
      Caption = 'SysTimer'
    end
    object labelSysTasksNum: TLabel
      Left = 171
      Top = 24
      Width = 79
      Height = 16
      Caption = 'SysTasksNum'
    end
    object labelSysReadyTasksNum: TLabel
      Left = 353
      Top = 24
      Width = 114
      Height = 16
      Caption = 'SysReadyTasksNum'
    end
    object stSysTimer: TStaticText
      Left = 70
      Top = 22
      Width = 74
      Height = 20
      Hint = #1041#1088#1086#1103#1095' '#1085#1072' '#1087#1088#1077#1074#1082#1083#1102#1095#1074#1072#1085#1080#1103#1090#1072
      Alignment = taRightJustify
      AutoSize = False
      BevelKind = bkFlat
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object stSysTasksNum: TStaticText
      Left = 252
      Top = 22
      Width = 74
      Height = 20
      Hint = #1054#1073#1097' '#1073#1088#1086#1081' '#1079#1072#1076#1072#1095#1080
      Alignment = taRightJustify
      AutoSize = False
      BevelKind = bkFlat
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
    end
    object stSysReadyTasksNum: TStaticText
      Left = 469
      Top = 22
      Width = 74
      Height = 20
      Hint = #1041#1088#1086#1081' '#1075#1086#1090#1086#1074#1080' '#1079#1072#1076#1072#1095#1080
      Alignment = taRightJustify
      AutoSize = False
      BevelKind = bkFlat
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
    end
  end
  object gboxTask3: TGroupBox
    Left = 0
    Top = 220
    Width = 563
    Height = 101
    Align = alTop
    Caption = ' Task 3 '
    TabOrder = 4
    object pbarTask32: TProgressBar
      Left = 2
      Top = 78
      Width = 559
      Height = 21
      Align = alBottom
      TabOrder = 0
    end
    object stTask32: TStaticText
      Left = 13
      Top = 35
      Width = 227
      Height = 20
      AutoSize = False
      BevelInner = bvNone
      BevelKind = bkFlat
      TabOrder = 1
    end
    object buttonSuspend3: TButton
      Left = 485
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Suspend'
      TabOrder = 2
      OnClick = buttonSuspend1Click
    end
    object buttonResume3: TButton
      Left = 485
      Top = 33
      Width = 75
      Height = 25
      Caption = 'Resume'
      TabOrder = 3
      OnClick = buttonResume1Click
    end
    object pbarTask31: TProgressBar
      Left = 2
      Top = 57
      Width = 559
      Height = 21
      Align = alBottom
      TabOrder = 4
    end
    object stTask31: TStaticText
      Left = 13
      Top = 16
      Width = 227
      Height = 20
      AutoSize = False
      BevelInner = bvNone
      BevelKind = bkFlat
      TabOrder = 5
    end
  end
end
