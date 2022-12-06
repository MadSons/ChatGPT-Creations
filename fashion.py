# Import the required modules
from keras.datasets import fashion_mnist
from keras.utils import to_categorical
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
import matplotlib.pyplot as plt
import numpy as np

# Load the Fashion-MNIST dataset
(x_train, y_train), (x_test, y_test) = fashion_mnist.load_data()

# Define the class names
class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']
# Print the class names
for i in range(10):
    print(f'Class {i}: {class_names[i]}')
    
# Reshape the data arrays
x_train = x_train.reshape((60000, 28, 28, 1))
x_test = x_test.reshape((10000, 28, 28, 1))

# Convert the target arrays to categorical format
y_train = to_categorical(y_train)
y_test = to_categorical(y_test)

# Define the CNN architecture
model = Sequential()
model.add(Conv2D(32, kernel_size=3, activation='relu', input_shape=(28, 28, 1)))
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(64, kernel_size=3, activation='relu'))
model.add(MaxPooling2D(pool_size=2))
model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dense(10, activation='softmax'))

# Compile the model
model.compile(optimizer='Adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(x_train, y_train, batch_size=32, epochs=10, validation_data=(x_test, y_test))

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
