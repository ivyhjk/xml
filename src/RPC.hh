<?hh // strict

namespace Ivyhjk\Xml;

use Exception;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
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
        $document = new DOMDocument('1.0', $encoding);

        $value = new Value(Vector{$parameters}, $document);
        $param = new Param(Vector{$value}, $document);

        $params = new Params(Vector{$param}, $document);

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
        try {
            $node = new SimpleXMLElement($xml);
        } catch (Exception $e) {
            throw new XmlException($e->getMessage());
        }

        $decoded = Vector{};
        $document = new DOMDocument();

        $paramsNode = Params::fromNode($node, $document);

        foreach ($paramsNode->getParameters() as $paramEntity) {
            $valueEntities = $paramEntity->getValues();

            $parsedValues = Value::parseValues($valueEntities);

            if ($parsedValues->count() === 1) {
                $decoded->add($parsedValues->firstValue());
            } else {
                $decoded->add($parsedValues);
            }
        }

        if ($decoded->count() === 1) {
            return $decoded->firstValue();
        } else {
            return $decoded;
        }
    }
}