Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Pressure Bricklet

    ' Callback subroutine for pressure reached callback
    Sub PressureReachedCB(ByVal sender As BrickletPressure, ByVal pressure As Integer)
        Console.WriteLine("Pressure: " + (pressure/1000.0).ToString() + " kPa")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim p As New BrickletPressure(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        p.SetDebouncePeriod(10000)

        ' Register pressure reached callback to subroutine PressureReachedCB
        AddHandler p.PressureReachedCallback, AddressOf PressureReachedCB

        ' Configure threshold for pressure "greater than 10 kPa"
        p.SetPressureCallbackThreshold(">"C, 10*1000, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
