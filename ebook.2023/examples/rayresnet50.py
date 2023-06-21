import ray
from PIL import Image
from torchvision.models import resnet50, ResNet50_Weights
from torchvision.transforms.functional import pil_to_tensor

@ray.remote
class RayResNet50:
    """Classify images using the ResNet-50 model from torchvision.
       This is a serial implementation that classifies files one at a time.

    Attributes:
        weights: The weights configuration for the ResNet-50 model.
        preprocess: The transform used to preprocess images for the model.
        model: The ResNet-50 model.
        cats: The category metadata for the model predictions.

    Methods:
        __init__: setup model for classification
        classify_image: Classifies an image using the loaded ResNet-50 model.
    """

    def __init__(self):
        # Load the model weights
        self.weights = ResNet50_Weights.DEFAULT
        # extract the transform to normalize images
        self.preprocess = self.weights.transforms()
        preprocess_img = ResNet50_Weights.DEFAULT.transforms()

        # Load the model
        self.model = resnet50(weights=self.weights)
        # Set model to eval mode
        self.model.eval()

        # Get the category metadata
        self.cats = ResNet50_Weights.DEFAULT.meta["categories"]

    def classify_image(self, file_path):
        """
        Classifies an image using the loaded ResNet-50 model.

        Args:
            file_path: The path to the image file to be classified.

        Returns:
            A list of the top predicted categories for the image.
        Raises:
            Exception: If there is an image format error.
        """
        # open image and convert to tensor
        image = Image.open(file_path)
        imgtensor = pil_to_tensor(image)

        try:
            # classification -- returns probabilities on 1000 classes
            img_preds = self.model(self.preprocess(imgtensor).unsqueeze(dim=0))
            # extact class names
            preds = [self.cats[idx] for idx in img_preds.argsort()[0].numpy()][::-1][:3]
            return preds
        except:
            raise
