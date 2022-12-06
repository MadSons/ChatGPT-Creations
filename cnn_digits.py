from keras.datasets import mnist
from keras.utils import to_categorical
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Dense, Flatten, Dropout
from keras.optimizers import Adam

import matplotlib.pyplot as plt
import numpy as np

# Load the MNIST dataset and split it into training and test sets
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Normalize the images by dividing by 255
x_train = x_train / 255
x_test = x_test / 255

# One-hot encode the labels
y_train = to_categorical(y_train)
y_test = to_categorical(y_test)

# Define the CNN architecture
model = Sequential()
model.add(Conv2D(32, kernel_size=3, activation='relu', input_shape=(28, 28, 1)))
model.add(Dropout(0.5))
model.add(Conv2D(32, kernel_size=3, activation='relu'))
model.add(Dropout(0.5))
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(64, kernel_size=3, activation='relu'))
model.add(Dropout(0.5))
model.add(Conv2D(64, kernel_size=3, activation='relu'))
model.add(Dropout(0.5))
model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))

# Compile the model
model.compile(optimizer=Adam(), loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(x_train, y_train, epochs=10, batch_size=32, validation_data=(x_test, y_test))

# Evaluate the model on the test set
test_loss, test_acc = model.evaluate(x_test, y_test)
print('Test accuracy:', test_acc)


# Use the model to predict the classes of the test set
predictions = model.predict(x_test)

# Get the predicted and true classes for each image in the test set
predicted_classes = np.argmax(predictions, axis=1)
true_classes = np.argmax(y_test, axis=1)

# Loop over the predicted and true classes for each image
count = 0
for i in range(len(predicted_classes)):
  # If the predicted and true classes do not match, display the image and its labels
  if predicted_classes[i] != true_classes[i] and count < 10:
    plt.imshow(x_test[i], cmap='gray')
    plt.title('True label: {}  Predicted label: {}'.format(true_classes[i], predicted_classes[i]))
    plt.show()
    count += 1

