Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Pressure Bricklet

    ' Callback subroutine for pressure callback (parameter has unit Pa)
    Sub PressureCB(ByVal sender As BrickletPressure, ByVal pressure As Integer)
        Console.WriteLine("Pressure: " + (pressure/1000.0).ToString() + " kPa")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim p As New BrickletPressure(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register pressure callback to subroutine PressureCB
        AddHandler p.Pressure, AddressOf PressureCB

        ' Set period for pressure callback to 1s (1000ms)
        ' Note: The pressure callback is only called every second
        '       if the pressure has changed since the last call!
        p.SetPressureCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
