from flask import Flask, jsonify, render_template, Response
import threading
import serial
import cv2
import numpy as np
from tensorflow.keras.models import load_model

# Serial Setup (assuming GPS via serial0)
ser = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Flask App
app = Flask(__name__)

# GPS Data
gps_data = {"latitude": 0.0, "longitude": 0.0}

# Model Setup
model = load_model('trained_model.h5')
IMG_SIZE = (224, 224)
classes = ['Tomato___Tomato_Yellow_Leaf_Curl_Virus', 'Tomato___Tomato_mosaic_virus',
           'Tomato___Target_Spot', 'Tomato___Spider_mites Two-spotted_spider_mite',
           'Tomato___Septoria_leaf_spot', 'Tomato___Leaf_Mold', 'Tomato___Late_blight',
           'Tomato___healthy', 'Tomato___Early_blight', 'Tomato___Bacterial_spot',
           'Potato___Late_blight', 'Potato___healthy', 'Potato___Early_blight']

# Video capture
cap = cv2.VideoCapture(0)

def read_gps():
    global gps_data
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            if "Latitude/Longitude" in line:
                parts = line.split(": ")[1]
                lat, lng = map(float, parts.split("/"))
                gps_data = {"latitude": lat, "longitude": lng}
        except Exception as e:
            print(f"GPS Error: {e}")

def gen_frames():
    while True:
        success, frame = cap.read()
        if not success:
            break

        img = cv2.resize(frame, IMG_SIZE)
        img = img.astype('float32') / 255.0
        img = np.expand_dims(img, axis=0)

        predictions = model.predict(img)
        predicted_class = classes[np.argmax(predictions)]

        cv2.putText(frame, f'{predicted_class}', (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        ret, buffer = cv2.imencode('.jpg', frame)
        frame = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/')
def index():
    return render_template("dashboard.html")

@app.route('/gps')
def gps():
    return jsonify(gps_data)

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

# Start background GPS thread
threading.Thread(target=read_gps, daemon=True).start()

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)

