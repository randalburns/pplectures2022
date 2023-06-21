import ray
from PIL import Image
from torchvision.models import resnet50, ResNet50_Weights

@ray.remote
class RRN50Classify:
    """Classify images using the ResNet-50 model from torchvision.

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

        # Load the model
        self.model = resnet50(weights=self.weights)
        # Set model to eval mode
        self.model.eval()

        # Get the category metadata
        self.cats = ResNet50_Weights.DEFAULT.meta["categories"]

    def classify_image(self, tensor):
        """
        Classifies an image using the loaded ResNet-50 model.

        Args:
            file_path: The path to the image file to be classified.

        Returns:
            A list of the top predicted categories for the image.
        Raises:
            Exception: If there is an image format error.
        """
        try:
            # classification -- returns probabilities on 1000 classes
            img_preds = self.model(tensor)
            # extact class names
            preds = [self.cats[idx] for idx in img_preds.argsort()[0].numpy()][::-1][:3]
            return preds
        except:
            raise
