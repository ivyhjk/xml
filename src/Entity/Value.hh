<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Caster;
use Ivyhjk\Xml\Contract\ValueType;
use Ivyhjk\Xml\Exception\InvalidNodeException;
use Ivyhjk\Xml\Exception\UnsupportedValueType;

/**
 * <methodName> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Value extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'value';

    /**
     * Description of what this does.
     *
     * @param Vector<mixed> $values The values to use.
     * @param DOMDocument The root node to generate new elements.
     *
     * @return void
     */
    public function __construct(
        private Vector<mixed> $values,
        DOMDocument $document
    ) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the inside tag values.
     *
     * @return Vector<mixed>
     */
    public function getValues() : Vector<mixed>
    {
        return $this->values;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $valueElement = $this
            ->getDocument()
            ->createElement('value');

        // Create the element with anything.
        foreach ($this->getValues() as $value) {
            $type = gettype($value);

            if ($type === 'integer') {
                $type = 'int';
            } elseif ($type === 'object' || $type === 'array') {
                $type = 'struct';
            }

            try {
                ValueType::assert($type);
            } catch (\UnexpectedValueException $e) {
                throw new UnsupportedValueType($type);
            }

            if ($type === 'struct') {
                $typeElement = null;

                if ( ! $value instanceof KeyedTraversable) {
                    throw new UnsupportedValueType(gettype($value));
                }

                $members = Vector{};

                foreach ($value as $memberName => $memberValue) {
                    $childValue = new Value(Vector{
                        $memberValue
                    }, $this->getDocument());

                    $member = new Member($memberName, $childValue, $this->getDocument());

                    $members->add($member);
                }

                $struct = new Struct($members, $this->getDocument());

                $typeElement = $struct->getElement();
            } else {
                // If is not struct always contain an string as value.
                $typeElement = $this->getDocument()->createElement($type, (string) $value);
            }

            $valueElement->appendChild($typeElement);
        }

        return $valueElement;
    }

    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Value
    {
        // Name is mandatory!.
        if ($node->getName() !== self::TAG_NAME) {
            throw new InvalidNodeException('Invalid tag name for "value".');
        }

        $children = $node->children();

        if ($children->count() < 1) {
            throw new InvalidNodeException('Value tag has no children.');
        }

        $values = Vector{};

        foreach ($children as $child) {
            $childName = $child->getName();

            if ($childName === 'struct') {
                $casted = null;

                $struct = Struct::fromNode($child, $document);

                $values->add($struct);
            } else {
                $casted = Caster::cast($child->getName(), $child);

                $values->add($casted);
            }
        }

        $value = new Value($values, $document);

        return $value;
    }

    /**
     * Parse a vector of Value into readable values.
     *
     * @param Vector<Ivyhjk\Xml\Entity\Value> $values The values to be parsed.
     *
     * @return Vector<mixed>
     */
    public static function parseValues(Vector<Value> $values) : Vector<mixed>
    {
        $parsedValues = Vector{};

        foreach ($values as $value) {
            $parsed = self::parseValue($value);

            $parsedValues->add($parsed);
        }

        return $parsedValues;
    }

    /**
     * Parse a Value objecto into a readable value.
     *
     * @param Ivyhjk\Xml\Entity\Value $value The value to be parsed.
     *
     * @return mixed
     */
    public static function parseValue(Value $value) : mixed
    {
        $parsedValues = Vector{};
        $childrenValues = $value->getValues();

        foreach ($childrenValues as $childValue) {
            if ($childValue instanceof Struct) {
                // Struct are objects, objects are KeyedTraversable.
                $parsedStruct = Map{};

                $members = $childValue->getMembers();

                foreach ($members as $member) {
                    $memberKey = $member->getName();
                    $parsedValue = self::parseValue($member->getValue());

                    $parsedStruct->set($memberKey, $parsedValue);
                }

                $parsedValues->add($parsedStruct);
            } else {
                $parsedValues->add($childValue);
            }
        }

        if ($parsedValues->count() === 1) {
            return $parsedValues->firstValue();
        }

        return $parsedValues;
    }
}
