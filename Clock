from seriffont import seriffont
from time import sleep
from ST7735 import TFT
from machine import SPI, Pin, RTC
import network, urequests, utime, machine, ntptime
spi = SPI(2, baudrate=20000000, polarity=0, phase=0, sck=Pin(14), mosi=Pin(13), miso=Pin(12))
tft = TFT(spi, 16, 17, 18)
tft.initr()
tft.rgb(True)
tft.fill(TFT.BLACK)
ds = 1
dm = 29
m = 3
H = 9
Min = 58
seg = 0
Año = 2025
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect("SSID", "PASSWORD")
while not wifi.isconnected():
    sleep(1)
rtc = RTC()
DS = ["Lunes    ", "Martes   ", "Miercoles", "Jueves   ", "Viernes  "]
DM = [f"{i:02}" for i in range(1, 32)]
M = ["Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
     "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"]
DTM = [30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30]
def ajustar_zona_horaria(offset_horas):
    año, mes, dia, semana, hora, minuto, segundo, _ = rtc.datetime()
    hora = (hora + offset_horas) % 24
    rtc.datetime((año, mes, dia, semana, hora, minuto, segundo, 0))
def actualizar_variables_reloj():
    global H, Min, seg, ds, dm, m
    (año, mes, dia, semana, hora, minuto, segundo, _) = rtc.datetime()
    H = hora
    Min = minuto
    seg = segundo
    dm = dia - 2 
    m = mes - 1 
    ds = semana -1 if semana < 5 else 0
    print(H,Min,seg,dm,m,ds)
def sincronizar_rtc():
    try:
        ntptime.settime()
        ajustar_zona_horaria(-5)  # Ecuador UTC-5
        actualizar_variables_reloj()
        print("RTC sincronizado con NTP")
        return True
    except Exception as ntp_e:
        print("Fallo NTP:", ntp_e)
        return False
if sincronizar_rtc():
    print("RTC ok")
else:
    print("use pred. time")
ultima_sincro = utime.time()
intervalo_sincro = 3600  # cada hora
print(H,Min,seg,dm,m,ds)
while True:
    if (utime.time() - ultima_sincro) >= intervalo_sincro:
        if sincronizar_rtc():
            ultima_sincro = utime.time()
    tft.fill(TFT.BLACK)
    periodo = "AM" if H < 12 else "PM"
    tft.text((5, 0), f"{DS[ds]} {DM[dm]}       {M[m]}  {Año}", TFT.WHITE, seriffont, 1, nowrap=False)
    tft.text((5, 30), f"{H}", TFT.WHITE, seriffont, 5, nowrap=False)
    tft.text((65, 30), ":  ", TFT.WHITE, seriffont, 5, nowrap=False)
    tft.text((80, 40), f"{Min}", TFT.WHITE, seriffont, 3, nowrap=False)
    tft.text((5, 80), f"{periodo}", TFT.WHITE, seriffont, 2, nowrap=False)
    if seg == 59:
        Min += 1
        seg = 0
    if Min == 59:
        Min = 0
        H += 1
    if H == 23:
        H = 0
        ds += 1
        dm += 1
    if ds == 4:
        ds = 0
    if dm == DTM[m]:
        dm = 0
        m += 1
    if m == 11:
        m = 0
        Año +=1
    sleep(1)
    seg += 1
