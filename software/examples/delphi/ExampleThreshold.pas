program ExampleThreshold;

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
    procedure PressureReachedCB(sender: TBrickletPressure; const pressure: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for pressure reached callback (parameter has unit Pa) }
procedure TExample.PressureReachedCB(sender: TBrickletPressure; const pressure: longint);
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

  { Get threshold callbacks with a debounce time of 10 seconds (10000ms) }
  p.SetDebouncePeriod(10000);

  { Register pressure reached callback to procedure PressureReachedCB }
  p.OnPressureReached := {$ifdef FPC}@{$endif}PressureReachedCB;

  { Configure threshold for pressure "greater than 10 kPa" (unit is Pa) }
  p.SetPressureCallbackThreshold('>', 10*1000, 0);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
