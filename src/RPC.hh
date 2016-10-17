<?hh // strict

namespace Ivyhjk\Xml;

use Exception;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
use Ivyhjk\Xml\Entity\MethodResponse;
use Ivyhjk\Xml\Exception\XmlException;

/**
 * XML RPC manager.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class RPC
{
    /**
     * Encode parameters into XML RPC.
     *
     * @param mixed $parameters RPC method args.
     * @param string $encoding The XML encoding.
     *
     * @return string
     */
    public static function encode(mixed $parameters, string $encoding = 'utf-8') : string
    {
        libxml_use_internal_errors(true);

        $document = new DOMDocument('1.0', $encoding);

        $givenParams = Vector{};

        if (is_array($parameters) && array_key_exists(0, $parameters)) {
            foreach ($parameters as $parameter) {
                $value = new Value(Vector{$parameter}, $document);
                $param = new Param(Vector{$value}, $document);

                $givenParams->add($param);
            }
        } else {
            $value = new Value(Vector{$parameters}, $document);
            $param = new Param(Vector{$value}, $document);
            $givenParams = Vector{$param};
        }

        $params = new Params($givenParams, $document);

        $document->appendChild($params->getElement());

        return $document->saveXML();
    }

    /**
     * Decode a XML RPC.
     *
     * @param string $xml
     *
     * @return mixed
     * @throws Ivyhjk\Xml\Exception\XmlException
     */
    public static function decode(string $xml) : mixed
    {
        libxml_use_internal_errors(true);

        try {
            $node = new SimpleXMLElement($xml);
        } catch (Exception $e) {
            throw new XmlException($e->getMessage());
        }

        $decoded = Vector{};
        $document = new DOMDocument();

        $processParams = (Params $paramsNode) ==> {
            foreach ($paramsNode->getParameters() as $paramEntity) {
                $valueEntities = $paramEntity->getValues();

                $parsedValues = Value::parseValues($valueEntities);

                if ($parsedValues->count() === 1) {
                    $decoded->add($parsedValues->firstValue());
                } else {
                    $decoded->add($parsedValues);
                }
            }
        };

        if ($node->getName() === MethodResponse::TAG_NAME) {
            $methodResponse = MethodResponse::fromNode($node, $document);

            foreach ($methodResponse->getParameters() as $params) {
                $processParams($params);
            }
        } else {
            $params = Params::fromNode($node, $document);

            $processParams($params);
        }

        if ($decoded->count() === 1) {
            return $decoded->firstValue();
        } else {
            return $decoded;
        }
    }
}
