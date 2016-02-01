program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletPressure;

type
  TExample = class
  private
    ipcon: TIPConnection;
    p: TBrickletPressure;
  public
    procedure PressureCB(sender: TBrickletPressure; const pressure: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for pressure callback (parameter has unit Pa) }
procedure TExample.PressureCB(sender: TBrickletPressure; const pressure: longint);
begin
  WriteLn(Format('Pressure: %f kPa', [pressure/1000.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  p := TBrickletPressure.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register pressure callback to procedure PressureCB }
  p.OnPressure := {$ifdef FPC}@{$endif}PressureCB;

  { Set period for pressure callback to 1s (1000ms)
    Note: The pressure callback is only called every second
          if the pressure has changed since the last call! }
  p.SetPressureCallbackPeriod(1000);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
