import ray
from PIL import Image
from torchvision.models import resnet50, ResNet50_Weights
from torchvision.transforms.functional import pil_to_tensor

@ray.remote
class RRN50Normalize:
    """Normalize JPEG images into tensors for the ResNet-50 model from torchvision.
  
    Methods:
        __init__: setup model for classification
        normalize_image: Normalizes an image using the ResNet-50 transform.
    """

    def __init__(self):
        # Load the model weights
        self.weights = ResNet50_Weights.DEFAULT
        # extract the transform to normalize images
        self.preprocess = self.weights.transforms()
        preprocess_img = ResNet50_Weights.DEFAULT.transforms()

    def normalize_image(self, file_path):
        """
        Normalizes an image prior to classification.

        Args:
            file_path: The path to the image file to be normalized.

        Returns:
            A ResNet50 normalized tensor to be classified
            
        Raises:
            Exception: If there is an image format error.
        """
        # open image and convert to tensor
        image = Image.open(file_path)
        imgtensor = pil_to_tensor(image)

        try:
            return self.preprocess(imgtensor).unsqueeze(dim=0)
        except:
            raise
