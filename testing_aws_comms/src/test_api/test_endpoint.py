import requests
import argparse
import json

def convert_dict_json(
        dictionary: dict  
    ):
    return json.dumps(dictionary)


if __name__ == "__main__":
    certificate_path = f"./src/keys/69178f6cf1c7bfaee3617f664441aff28371606260d4e6313097b8f293ea2c50-certificate.pem.crt"
    private_key_path = f"./src/keys/69178f6cf1c7bfaee3617f664441aff28371606260d4e6313097b8f293ea2c50-private.pem.key"
    public_key_path = f"./src/keys/69178f6cf1c7bfaee3617f664441aff28371606260d4e6313097b8f293ea2c50-public.pem.key"
    end_point = f"a1tmhchnoirllr-ats.iot.us-east-1.amazonaws.com"

    parser = argparse.ArgumentParser(description='Test shadow API')
    topic = "/test/insert_into_table"

    parser.add_argument(
        '--endpoint', 
        required=False, 
        default=end_point,
        help='The AWS IoT endpoint'
    )
    parser.add_argument(
        '--cert', required=False, 
        default=certificate_path, 
        help='Path to certificate'
    )
    parser.add_argument(
        '--key', required=False, 
        default=private_key_path, 
        help='Path to private key'
    )
    parser.add_argument(
        '--topic',
         required=False,
          default=topic, 
          help='Path of topic for the message to be posted'
        )
    parser.add_argument(
        '--message',
        required=False,
        default=''
    )

    args = parser.parse_args()
    publish_url = 'https://' + args.endpoint + ':8443/topics/' + args.topic + '?qos=1'
    print(publish_url)

    message={
        "device_id":"weather_1",
        "timestamp":2,
        "state":{
            "reported":{
                "humidity":0.29,
                "temp":25
            }
        }
    }

    message = convert_dict_json(
        dictionary=message
    )

    #publish_msg = message.encode('utf-8') 
    publish = requests.request(
            'POST',
            publish_url,
            data=message,
            cert=[args.cert, args.key]
        )
    print("Response status: ", str(publish.status_code))
    if publish.status_code == 200:
        print("Response body:", publish.text)
    else:
        print("Response body:", publish.text)