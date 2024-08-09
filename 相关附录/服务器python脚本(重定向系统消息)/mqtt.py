import paho.mqtt.client as mqtt

# MQTT 服务器的地址
broker_address = "www.dongnets.asia"

# 回调函数，当连接到服务器时调用
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # 订阅主题
        client.subscribe("$SYS/brokers/emqx@172.17.0.2/clients/Bedroom_Light/#")
    else:
        print("Failed to connect, return code %d\n", rc)
# MQTT 回调函数：接收到消息
def on_message(client, userdata, msg):
    #print(f"Received message '{msg.payload}' on topic '{msg.topic}' with QoS {msg.qos}")
    # 处理消息
    if msg.topic == "$SYS/brokers/emqx@172.17.0.2/clients/Bedroom_Light/disconnected":
        print("Bedroom Light is disconnected")
        # 发送设备离线
        client.publish("DHT_state", "offline", qos=0) 
        client.publish("ESP_state", "offline", qos=0)
    elif msg.topic == "$SYS/brokers/emqx@172.17.0.2/clients/Bedroom_Light/connected":
        print("Bedroom Light is connected")
        # 发送设备在线
        client.publish("ESP_state", "online", qos=0)

# 创建 MQTT 客户端实例
client = mqtt.Client()

# 指定连接到服务器时的回调函数
client.on_connect = on_connect
# 设置接收到消息的回调函数
client.on_message = on_message
# 设置用户名
client.username_pw_set("python_esp", "public")

# 连接到 MQTT 服务器
client.connect(broker_address, 1883, 60)

# 开始循环以处理网络流量和自动重新连接
client.loop_forever()