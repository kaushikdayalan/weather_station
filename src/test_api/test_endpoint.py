import requests
import argparse

parser = argparse.ArgumentParser(description='Test shadow API')
parser.add_argument('--endpoint', required=True, help='The AWS IoT endpoint')

