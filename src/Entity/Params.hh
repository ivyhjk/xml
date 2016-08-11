<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Params extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'params';

    /**
     * Generate new <params> tag wrapper.
     *
     * @param Vector<Ivyhjk\Xml\Entity\Param> $parameters the associated parameters.
     * @param DOMDocument The root node.
     *
     * @return void
     */
    public function __construct(private Vector<Param> $parameters, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the tag/DOMElement name.
     *
     * @return string
     */
    public function getParameters() : Vector<Param>
    {
        return $this->parameters;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this
            ->getDocument()
            ->createElement('params');

        foreach ($this->getParameters() as $parameter) {
            $element->appendChild($parameter->getElement());
        }

        return $element;
    }

    /**
     * Description of what this does.
     *
     * @param mixed $myVar Short description
     *
     * @return Ivyhjk\Xml\Entity\Params
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Params
    {
        // Name is mandatory!
        if ($node->getName() !== self::TAG_NAME) {
            throw new InvalidNodeException('Missing node "methodName"');
        }

        // Get the params.
        $paramNodes = new Vector($node->xpath('param'));

        $paramEntities = Vector{};

        foreach ($paramNodes as $paramNode) {
            $paramEntity = Param::fromNode($paramNode, $document);

            $paramEntities->add($paramEntity);
        }

        return  new Params($paramEntities, $document);
    }
}
