Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Pressure Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim p As New BrickletPressure(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current pressure (unit is Pa)
        Dim pressure As Integer = p.GetPressure()
        Console.WriteLine("Pressure: " + (pressure/1000.0).ToString() + " kPa")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
