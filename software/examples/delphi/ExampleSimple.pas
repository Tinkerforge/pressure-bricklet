program ExampleSimple;

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
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Pressure Bricklet }

var
  e: TExample;

procedure TExample.Execute;
var pressure: longint;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  p := TBrickletPressure.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current pressure }
  pressure := p.GetPressure;
  WriteLn(Format('Pressure: %f kPa', [pressure/1000.0]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
