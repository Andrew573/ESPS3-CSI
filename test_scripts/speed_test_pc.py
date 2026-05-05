import serial
import time

PORT = 'COM9'
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=10)
time.sleep(2)

print("=== ESP32 -> PC 速度测试 ===\n")

ser.reset_input_buffer()
ser.write(b'START\n')

received = 0
start = time.time()
last_time = start

while True:
    if ser.in_waiting:
        data = ser.read(ser.in_waiting)
        received += len(data)

        current = time.time()
        if current - last_time >= 1.0:
            elapsed = current - start
            speed = received / 1024 / elapsed
            print(f"已接收: {received/1024:.1f} KB  |  速度: {speed:.1f} KB/s")
            last_time = current

    elapsed = time.time() - start
    if elapsed >= 10:
        break

speed = received / 1024 / elapsed

print(f"\n=== 测试完成 ===")
print(f"总接收: {received/1024:.1f} KB ({received/1024/1024:.2f} MB)")
print(f"耗时: {elapsed:.2f} 秒")
print(f"平均速度: {speed:.1f} KB/s")

ser.close()