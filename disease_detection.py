import cv2
import numpy as np
from tensorflow.keras.models import load_model

# Loading trained model
model = load_model('trained_model.h5')

# Defining your input image size
IMG_SIZE = (224, 224)

# Class labels
classes = ['Tomato___Tomato_Yellow_Leaf_Curl_Virus', 'Tomato___Tomato_mosaic_virus', 'Tomato___Target_Spot', 'Tomato___Spider_mites Two-spotted_spider_mite', 'Tomato___Septoria_leaf_spot', 'Tomato___Leaf_Mold', 'Tomato___Late_blight', 'Tomato___healthy', 'Tomato___Early_blight', 'Tomato___Bacterial_spot', 'Potato___Late_blight', 'Potato___healthy', 'Potato___Early_blight']

# Start video capture (0 = default camera)
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Preprocess the frame
    img = cv2.resize(frame, IMG_SIZE)
    img = img.astype('float32') / 255.0
    img = np.expand_dims(img, axis=0)

    # Predict
    predictions = model.predict(img)
    predicted_class = classes[np.argmax(predictions)]

    # Display prediction
    cv2.putText(frame, f'Prediction: {predicted_class}', (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    cv2.imshow("Plant Disease Detection", frame)

    # Break on 'q' key
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
